// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#include "BciControllerPrivatePCH.h"

#include "Engine.h"
#include "BciActor.h"

#include "BciControllerPlugin.h"
#include "BciControllerInputDevice.h"

DEFINE_LOG_CATEGORY_STATIC(LogBciActor, Log, All);

// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

ABciActorObject::ABciActorObject(const class FObjectInitializer& PCIP) : Super(PCIP) {
    UE_LOG(LogBciActor, Log, TEXT("Construct"));
}
 
void ABciActorObject::FunkyMethod() {
    UE_LOG(LogBciActor, Log, TEXT("FunkyMethod()"));
}