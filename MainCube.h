// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "GameFramework/Pawn.h"
#include "MainCube.generated.h"


UCLASS()
class THECUBEONE_API AMainCube : public APawn, public IInteraction
{
	GENERATED_BODY()
	
public:
	AMainCube();
	virtual void SetScale_Implementation(FVector MaxScale, FVector IncreaseVector);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Movement functions;
	void MoveForwardStart();
	void MoveBackwardStart();
	void MoveRightStart();
	void MoveLeftStart();
	
	void MoveInDirection();
	void StopMoving();
	void SetLocation();
	void MakeMoveInDirection();

	void LookUp(float Value);
	void LookRight(float Value);

	void Interaction();

	float MovementInterval;

	FVector fvDirection;
	FVector fvLocationFrom;

	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MyStaticMesh;

private:


	FTimerHandle MovementTimer;
	FTimerHandle MovementTimerRecheck;
	FTimerHandle TimerHandle;

	bool bIsMoving;
	bool bIsMovingSameSpeed;
	
	float fCount;
	float fVectorIncrease;

	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpotLightComponent* SpotLightComponent;
};
