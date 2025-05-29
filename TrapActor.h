// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TrapActor.generated.h"

UCLASS()
class THECUBEONE_API ATrapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapActor();
	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//bool bHasExecuted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//void SpawnCube();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorPlace")
	FVector PlaceToFall;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

private:
	FTimerHandle TimerHandle;
};
