// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction.h"
#include "InteractionActor.generated.h"

UCLASS(Blueprintable, Placeable)
class THECUBEONE_API AInteractionActor : public AActor, public IInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionActor();
	virtual void Interact_Implementation() override;

protected:
	void SetScale();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetScale")
	FVector CubeMaxScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetScale")
	FVector CubeIncrease;
	

};
