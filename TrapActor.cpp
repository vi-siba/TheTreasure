// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor.h"

// Sets default values
ATrapActor::ATrapActor()
{
}

// Called when the game starts or when spawned
void ATrapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
void ATrapActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
	}
}

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Overlaped"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	CollisionBox->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::OnOverlapBegin);

	PlaceToFall = FVector();
    bHasExecuted = false;

*/