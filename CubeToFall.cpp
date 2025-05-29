// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeToFall.h"

// Sets default values
ACubeToFall::ACubeToFall()
{
   // CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    if (CubeMesh != nullptr)
    {
        RootComponent = CubeMesh;  
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CubeMesh was not created successfully!"));
    }
	
}

// Called when the game starts or when spawned
void ACubeToFall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeToFall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

