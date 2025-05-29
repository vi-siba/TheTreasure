// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interaction.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteraction : public UInterface
{
    GENERATED_BODY()
};

class THECUBEONE_API IInteraction
{
    GENERATED_BODY()  

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact();
    

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void SetScale(FVector MaxScale, FVector IncreaseVector);

};