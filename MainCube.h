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
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void MoveForwardStart();
    void MoveBackwardStart();
    void MoveRightStart();
    void MoveLeftStart();

    void MoveInDirection();
    void StopMoving();
    void SetLocation();
    void MakeMoveInDirection();

    void Interaction();

    float MovementInterval;

    FVector fvDirection;
    FVector fvLocationFrom;

    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
    UStaticMeshComponent* MyStaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    TArray<TSubclassOf<AActor>> IgnoredClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    TArray<AActor*> IgnoredActors;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ForceStopMovement();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetMovementStop();

protected:
    bool ShouldIgnoreActor(AActor* ActorToCheck) const;

private:
    FTimerHandle MovementTimer;
    FTimerHandle MovementTimerRecheck;
    FTimerHandle TimerHandle;

    bool bIsMoving;
    bool bIsMovingSameSpeed;
    bool bMovementForcedStopped;

    bool bCheckCollisionThisFrame;

    float CurrentStepInterval;

    float fCount;
    float fVectorIncrease;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* RootSceneComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USpotLightComponent* SpotLightComponent;

};