#pragma once
#include "GameFramework/Actor.h"
#include "BciActor.generated.h"

UCLASS(MinimalAPI, hidecategories = (Input))
class ABciActorObject : public AActor
{
    GENERATED_UCLASS_BODY()

        UFUNCTION(BlueprintCallable, Category = "Development")
        virtual void FunkyMethod();
};