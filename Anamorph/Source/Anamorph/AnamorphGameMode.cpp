// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Anamorph.h"
#include "AnamorphGameMode.h"
#include "AnamorphHUD.h"
#include "AnamorphCharacter.h"

AAnamorphGameMode::AAnamorphGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAnamorphHUD::StaticClass();
}
