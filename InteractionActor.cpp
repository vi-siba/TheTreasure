// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionActor.h"

// Sets default values
AInteractionActor::AInteractionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeIncrease = FVector(1.0f, 0.0f, 0.0f);
	CubeMaxScale = FVector(2.0f, 1.0f, 1.0f);

	
}

// Called when the game starts or when spawned
void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionActor::Interact_Implementation()
{

	SetScale();
}

void AInteractionActor::SetScale()
{
	AActor* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	IInteraction* Interaction = Cast<IInteraction>(PlayerCharacter);

	if (Interaction) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("I interact"));

		Interaction->Execute_SetScale(PlayerCharacter, CubeMaxScale, CubeIncrease);
	}
				
}

