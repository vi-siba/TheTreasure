// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCube.h"
#include <iostream>
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SpotLightComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include <cmath>


AMainCube::AMainCube()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
	RootComponent->SetMobility(EComponentMobility::Movable);

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(RootComponent);


	SpotLightComponent->SetRelativeLocation(FVector(-150.0f, 150.0f, 270.0f)); 
	SpotLightComponent->SetRelativeRotation(FRotator(-50.0f, -45.0f, 0.0f));
	SpotLightComponent->SetIntensity(1000.0f); 
	SpotLightComponent->SetOuterConeAngle(44.0f); // Set the cone angle
	SpotLightComponent->SetInnerConeAngle(0.0f); // Set the inner cone angle
	SpotLightComponent->SetLightColor(FLinearColor::White); // Set light color


	PrimaryActorTick.bCanEverTick = true;
	bIsMoving = false;	
	bIsMovingSameSpeed = false;

	fCount = 0.0f;
	fVectorIncrease = 0.0f;
	MovementInterval = 0.1f;
}

void AMainCube::SetScale_Implementation(FVector MaxScale, FVector IncreaseVector)
{
	if (MyStaticMesh != nullptr)
	{
	if (MaxScale.X != MyStaticMesh->GetComponentScale().X || MaxScale.Y != MyStaticMesh->GetComponentScale().Y || MaxScale.Z != MyStaticMesh->GetComponentScale().Z)
	{
		FVector CurrentScale = MyStaticMesh->GetComponentScale();
		FVector Increase(0.1f, 0.1f, 0.1f);
		Increase*= IncreaseVector;
		MyStaticMesh->SetWorldScale3D(CurrentScale + Increase);
	}
	}

}

// Called to bind functionality to input
void AMainCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &AMainCube::MoveForwardStart);
	PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &AMainCube::MoveBackwardStart);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &AMainCube::MoveRightStart);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMainCube::MoveLeftStart);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMainCube::Interaction);
	
	PlayerInputComponent->BindAction("MoveForward", IE_Released, this, &AMainCube::StopMoving);
	PlayerInputComponent->BindAction("MoveBackward", IE_Released, this, &AMainCube::StopMoving);
	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &AMainCube::StopMoving);
	PlayerInputComponent->BindAction("MoveLeft", IE_Released, this, &AMainCube::StopMoving);




	PlayerInputComponent->BindAxis("LookUp", this, &AMainCube::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMainCube::LookRight);


}

void AMainCube::BeginPlay()
{
		Super::BeginPlay();
}

void AMainCube::LookUp(float Value)
{
	if (Controller != nullptr)
	{

	}
}
void AMainCube::LookRight(float Value)
{
	if (Controller != nullptr)
	{

	}
}

//Возможно можно удалить эту функцию и в каждом Move переписать;
void AMainCube::MoveInDirection()
{
	if (Controller != nullptr)
	{
		if (!bIsMoving)
		{
		bIsMoving = true;
		MakeMoveInDirection();
		}
		
	}

}

void AMainCube::MakeMoveInDirection()
{
	float DoubleCount = 2 * fCount;
	if ((fabs(DoubleCount - round(DoubleCount)))< 1e-3f)
	{
		//FVector ActorScale = GetActorRelativeScale3D();
		FVector StartLocationTracing = GetActorLocation();
		FVector EndLocationTracing = GetActorLocation()+fvDirection*FVector(100.0f, 100.0f, 50.0f);
		FVector TraceBoxSize = GetActorRelativeScale3D() * FVector(50.0f, 50.0f, 50.0f);
			//FVector(60.0f, 60.0f, 50.0f);
		FRotator Orientation = GetActorRotation();
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
		FHitResult HitResult;

		bool bHit = UKismetSystemLibrary::BoxTraceSingle(this, StartLocationTracing, EndLocationTracing, TraceBoxSize, Orientation, TraceChannel, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, HitResult, true);

		if (bHit)
		{

			if(fCount<0.01f)
			{
				AMainCube::StopMoving();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Less!"));
				bIsMoving = false;

			}
			else
			{
			AMainCube::StopMoving();
			AMainCube::SetLocation();
			}
		}
		else
		{
			AMainCube::SetLocation();
		}
	}
	else
	{
		AMainCube::SetLocation();
	}
}

void AMainCube::SetLocation()
{
	fCount += 0.01f;

	if (fCount <= 0.5f)
	{
		fVectorIncrease = ((fCount) * (fCount) * (3.0f - 2.0f * (fCount)));
		SetActorLocation((fvLocationFrom + 100.0f * fVectorIncrease * (fvDirection)), false);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCube::MakeMoveInDirection, 0.005f, true);
	}
	else
	{
		if ((GetWorldTimerManager().IsTimerActive(MovementTimer)))
		{
			fCount += 0.01f;
			bIsMovingSameSpeed = true;
			fVectorIncrease = (fCount);
			SetActorLocation((fvLocationFrom + 100.0f * fVectorIncrease * (fvDirection)), false);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCube::MakeMoveInDirection, 0.006f, true);
		}

		else
		{
			if ((fCount)-0.01f > ((floor(fCount))))
			{
				if (bIsMovingSameSpeed)
				{
					float f;
					for (f = floor(fCount); (f - floor(f)) * (f - floor(f)) * (3.0f - 2.0f * (f - floor(f))) + floor(f) < fCount; f += 0.01f)
					{
					}
					fCount = f;
					bIsMovingSameSpeed = false;
				}

				fVectorIncrease = ((fCount - floor(fCount)) * (fCount - floor(fCount)) * (3.0f - 2.0f * (fCount - floor(fCount)))) + floor(fCount);
				SetActorLocation((fvLocationFrom + 100.0f * fVectorIncrease * (fvDirection)), false);
				GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCube::MakeMoveInDirection, 0.002f, true);
			}
			else
			{
				bIsMovingSameSpeed = false;
				GetWorldTimerManager().ClearTimer(TimerHandle);
				fVectorIncrease = floor(fCount);
				SetActorLocation((fvLocationFrom + 100 * fVectorIncrease * (fvDirection)), false);
				bIsMoving = false;
				fCount = 0.0f;
				fVectorIncrease = 0.0f;
			}

		}
	}
}

void AMainCube::StopMoving()
{
	GetWorldTimerManager().ClearTimer(MovementTimer);
	GetWorldTimerManager().ClearTimer(MovementTimerRecheck);
}

void AMainCube::MoveForwardStart()
{

	if(!bIsMoving)
	{
	fvDirection = FVector::ForwardVector;
	fvLocationFrom = (GetActorLocation());
	MoveInDirection();
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveForwardStart, MovementInterval, true);
	}

	else
	{
	GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveForwardStart, MovementInterval / 2.0f, true);
	}

}

void AMainCube::MoveBackwardStart()
{
	if (!bIsMoving)
	{
	fvDirection = -FVector::ForwardVector;
	fvLocationFrom = (GetActorLocation());
	MoveInDirection();
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveBackwardStart, MovementInterval, true);
	}
	
	else
	{
	GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveBackwardStart, MovementInterval / 2.0f, true);
	}
}
	
void AMainCube::MoveLeftStart()
{
	if (!bIsMoving)
	{
	fvDirection = -FVector::RightVector;
	fvLocationFrom = (GetActorLocation());
	MoveInDirection();
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveLeftStart, MovementInterval, true);
	}
	
	else
	{
	GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveLeftStart, MovementInterval / 2.0f, true);
	}
}

void AMainCube::MoveRightStart()
{
	if (!bIsMoving)
	{
	fvDirection = FVector::RightVector;
	fvLocationFrom = (GetActorLocation());
	MoveInDirection();
	GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveRightStart, MovementInterval, true);
	}
	
	else
	{
	GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveRightStart, MovementInterval / 2.0f, true);
	}
}

void AMainCube::Interaction()
{

	// Define the start and end locations of the trace
	FVector LocationToCheck = GetActorLocation();
	FVector TraceBoxSize = FVector(60.0f, 60.0f, 55.0f);
	FRotator Orientation = GetActorRotation();

	// Define the trace channel (e.g., Visibility or Custom)
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		this, // World context object
		LocationToCheck, // Start location tracign
		LocationToCheck, // End location tracing
		TraceBoxSize,
		Orientation, // Box orientation
		TraceChannel, // Trace channel
		false, // Trace complex (false for simple collision)
		TArray<AActor*>(), // Actors to ignore
		EDrawDebugTrace::ForDuration, // Debug drawing (optional)
		HitResult, // Out parameter for hit result
		true // Ignore self
	);
	
	
	// Check if the trace hit something
	if (bHit)
	{
		if (bHit && HitResult.GetActor())
		{
			AActor* HitActor = HitResult.GetActor();

			// Check if actor implements the Interaction interface
			if (HitActor->GetClass()->ImplementsInterface(UInteraction::StaticClass()))
			{

				// Call Interact() function on the hit actor
				IInteraction* Interaction = Cast<IInteraction>(HitActor);
				Interaction->Execute_Interact(HitActor);

				/*if (Interaction)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("hitted!"));

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Doesnt have interface!"));
				}*/
			}
			
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Doesnt have!"));
			}
		}
	}
	
	
		

}

