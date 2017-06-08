// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Anamorph.h"
#include "AnamorphCharacter.h"
#include "AnamorphProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Log, All);

//////////////////////////////////////////////////////////////////////////
// AAnamorphCharacter

AAnamorphCharacter::AAnamorphCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AAnamorphCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAnamorphCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAnamorphCharacter::OnResetVR);

    PlayerInputComponent->BindAxis("MoveForward", this, &AAnamorphCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AAnamorphCharacter::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &AAnamorphCharacter::MoveUp);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAnamorphCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AAnamorphCharacter::LookUpAtRate);
}

void AAnamorphCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAnamorphCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AAnamorphCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}

	TouchItem.bIsPressed = false;
}


void AAnamorphCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AAnamorphCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        UPawnMovementComponent* MovementComponent = GetMovementComponent();

        if (MovementComponent)
        {
            // On left/right movement, rotate around the zero origin
            //
            FVector location = this->GetActorLocation();
            FVector rotatedVector = location.RotateAngleAxis(Value, FVector(0,0,1));
            FVector movement = location - rotatedVector;

            MovementComponent->AddInputVector(movement);

            // Point the camera's eyes towards the origin
            //
            FVector currentLoc;
            FRotator currentRot;
            this->GetActorEyesViewPoint(currentLoc, currentRot);

            FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(location, FVector(0,0,0));

            float rotation = currentRot.Yaw - PlayerRot.Yaw;

            if (rotation > 180.0 || rotation < -180)
            {
                rotation = PlayerRot.Yaw - currentRot.Yaw;
            }

            FString flocString = location.ToCompactString();
            FString fcurString = currentRot.ToCompactString();
            FString frotString = PlayerRot.ToCompactString();
            const TCHAR* locString = *flocString;
            const TCHAR* curString = *fcurString;
            const TCHAR* rotString = *frotString;

            float requiredYaw = PlayerRot.Yaw >= 0 ? PlayerRot.Yaw : 360.0 + PlayerRot.Yaw;
            float yawInput = requiredYaw - currentRot.Yaw;
            float normalisedYawInput = yawInput;

            UE_LOG(LogFPChar, Error, TEXT("Pos=%s, CurLookAt=%s, LookAtZero=%s, Yaw=%f"), locString, curString, rotString, yawInput);

            if(yawInput > 180.0) normalisedYawInput = yawInput - 360.0;
            else if(yawInput < -180.0) normalisedYawInput = 360.0 + yawInput;

            UE_LOG(LogFPChar, Error, TEXT("Req=%f, ReqNormd=%f, Current=%f, YawInput=%f, YawNormd=%f"), PlayerRot.Yaw, requiredYaw, currentRot.Yaw, yawInput, normalisedYawInput);

            //this->AddControllerPitchInput(currentRot.Pitch - PlayerRot.Pitch);
            this->AddControllerYawInput(normalisedYawInput / 100);

            //this->FaceRotation(PlayerRot);

            // Make sure we don't stray too far from the ideal viewing distance
            //
            float distance = FVector::Dist2D(location, FVector(0, 0, 0));
            float impulse = (distance - 3000) / 10;

            AddMovementInput(GetActorForwardVector(), impulse);
        }

        // add movement in that direction
        //AddMovementInput(GetActorRightVector(), Value);
    }
}

void AAnamorphCharacter::MoveUp(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorUpVector(), Value);
    }
}

void AAnamorphCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAnamorphCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AAnamorphCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAnamorphCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AAnamorphCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAnamorphCharacter::TouchUpdate);
	}
	return bResult;
}
