
// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCube.h"
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
    SpotLightComponent->SetOuterConeAngle(44.0f);
    SpotLightComponent->SetInnerConeAngle(0.0f);
    SpotLightComponent->SetLightColor(FLinearColor::White);

    PrimaryActorTick.bCanEverTick = true;
    bIsMoving = false;
    bIsMovingSameSpeed = false;
    bMovementForcedStopped = false;

    fCount = 0.0f;
    fVectorIncrease = 0.0f;
    MovementInterval = 0.1f;
}

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

}

void AMainCube::BeginPlay()
{
    Super::BeginPlay();
}


void AMainCube::MoveInDirection()
{
    if (Controller != nullptr && !bIsMoving && !bMovementForcedStopped)
    {
        bIsMoving = true;
        MakeMoveInDirection();
    }
}

void AMainCube::MakeMoveInDirection()
{

    if (bMovementForcedStopped)
    {


        StopMoving();
        bIsMoving = false;
        return;
    }

    const float DoubleCount = 2 * fCount;
    if (FMath::IsNearlyEqual(DoubleCount, FMath::RoundToFloat(DoubleCount), 1e-3f))
    {
        FVector StartLocationTracing = GetActorLocation();
        FVector EndLocationTracing = GetActorLocation() + fvDirection * FVector(100.0f, 100.0f, 50.0f);
        FVector TraceBoxSize = GetActorRelativeScale3D() * FVector(50.0f, 50.0f, 50.0f);
        FRotator Orientation = GetActorRotation();

        FHitResult HitResult;
        bool bHit = UKismetSystemLibrary::BoxTraceSingle(
            this,
            StartLocationTracing,
            EndLocationTracing,
            TraceBoxSize,
            Orientation,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false,
            IgnoredActors,
            EDrawDebugTrace::ForDuration,
            HitResult,
            true
        );

        if (bHit && HitResult.GetActor())
        {
            FString DebugMessage = FString::Printf(TEXT("Столкновение с: %s"), *HitResult.GetActor()->GetName());
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);
        }



        if (bHit && !ShouldIgnoreActor(HitResult.GetActor()))
        {
            if (fCount < 0.01f)
            {
                StopMoving();
                bIsMoving = false;
            }
            else
            {
                StopMoving();
                SetLocation();
            }
        }
        else
        {
            SetLocation();
        }
    }
    else
    {
        SetLocation();
    }
}

bool AMainCube::ShouldIgnoreActor(AActor* ActorToCheck) const
{
    if (!ActorToCheck)
    {
        return false;
    }

    for (const TSubclassOf<AActor>& IgnoredClass : IgnoredClasses)
    {
        if (ActorToCheck->IsA(IgnoredClass))
        {
            return true;
        }
    }
    return false;
}

void AMainCube::SetLocation()
{
    if (bMovementForcedStopped)
    {
        bIsMoving = false;
        return;
    }

    fCount += 0.01f;

    if (fCount <= 0.5f)
    {
        fVectorIncrease = ((fCount) * (fCount) * (3.0f - 2.0f * (fCount)));
        SetActorLocation((fvLocationFrom + 100.0f * fVectorIncrease * (fvDirection)), false);

        if (!bMovementForcedStopped)
        {
            GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCube::MakeMoveInDirection, 0.005f, true);
        }
        else
        {
            bIsMoving = false;
        }
    }
    else
    {
        if (GetWorldTimerManager().IsTimerActive(MovementTimer))
        {
            fCount += 0.01f;
            bIsMovingSameSpeed = true;
            fVectorIncrease = (fCount);
            SetActorLocation((fvLocationFrom + 100.0f * fVectorIncrease * (fvDirection)), false);

            if (!bMovementForcedStopped)
            {
                GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCube::MakeMoveInDirection, 0.006f, true);
            }
            else
            {
                bIsMoving = false;
            }
        }
        else
        {
            if ((fCount)-0.01f > (FMath::Floor(fCount)))
            {
                if (bIsMovingSameSpeed)
                {
                    float f;
                    for (f = FMath::Floor(fCount);
                        (f - FMath::Floor(f)) * (f - FMath::Floor(f)) * (3.0f - 2.0f * (f - FMath::Floor(f))) + FMath::Floor(f) < fCount;
                        f += 0.01f)
                    {
                    }
                    fCount = f;
                    bIsMovingSameSpeed = false;
                }

                fVectorIncrease = ((fCount - FMath::Floor(fCount)) * (fCount - FMath::Floor(fCount)) * (3.0f - 2.0f * (fCount - FMath::Floor(fCount)))) + FMath::Floor(fCount);
                SetActorLocation((fvLocationFrom + 100.0f * fVectorIncrease * (fvDirection)), false);

                if (!bMovementForcedStopped)
                {
                    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCube::MakeMoveInDirection, 0.005f, true);
                }
                else
                {
                    bIsMoving = false;
                }
            }
            else
            {
                bIsMovingSameSpeed = false;
                GetWorldTimerManager().ClearTimer(TimerHandle);
                fVectorIncrease = FMath::Floor(fCount);
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

void AMainCube::ForceStopMovement()
{
    FString DebugMessage = FString::Printf(TEXT("Hello World"));

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);

    StopMoving();

    bMovementForcedStopped = true;
    bIsMoving = false;
    bIsMovingSameSpeed = false;

    GetWorldTimerManager().ClearTimer(MovementTimer);
    GetWorldTimerManager().ClearTimer(MovementTimerRecheck);
    GetWorldTimerManager().ClearTimer(TimerHandle);

    fCount = 0.0f;
    fVectorIncrease = 0.0f;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }
}

void AMainCube::ResetMovementStop()
{
    bMovementForcedStopped = false;
}

void AMainCube::MoveForwardStart()
{
    if (bMovementForcedStopped)
    {
        StopMoving();
        return;
    }

    if (!bIsMoving)
    {
        fvDirection = FVector::ForwardVector;
        fvLocationFrom = GetActorLocation();
        MoveInDirection();
        GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveForwardStart, MovementInterval, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(MovementTimerRecheck);
        GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveForwardStart, MovementInterval / 2.0f, true);
    }
}

void AMainCube::MoveBackwardStart()
{
    if (bMovementForcedStopped)
    {
        StopMoving();
        return;
    }

    if (!bIsMoving)
    {
        fvDirection = -FVector::ForwardVector;
        fvLocationFrom = GetActorLocation();
        MoveInDirection();
        GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveBackwardStart, MovementInterval, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(MovementTimerRecheck);
        GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveBackwardStart, MovementInterval / 2.0f, true);
    }
}

void AMainCube::MoveLeftStart()
{


    if (bMovementForcedStopped)
    {
        StopMoving();
        return;
    }

    if (!bIsMoving)
    {
        fvDirection = -FVector::RightVector;
        fvLocationFrom = GetActorLocation();
        MoveInDirection();
        GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveLeftStart, MovementInterval, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(MovementTimerRecheck);
        GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveLeftStart, MovementInterval / 2.0f, true);
    }
}

void AMainCube::MoveRightStart()
{


    if (bMovementForcedStopped)
    {
        StopMoving();
        return;
    }

    if (!bIsMoving)
    {
        fvDirection = FVector::RightVector;
        fvLocationFrom = GetActorLocation();
        MoveInDirection();
        GetWorldTimerManager().SetTimer(MovementTimer, this, &AMainCube::MoveRightStart, MovementInterval, true);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(MovementTimerRecheck);
        GetWorldTimerManager().SetTimer(MovementTimerRecheck, this, &AMainCube::MoveRightStart, MovementInterval / 2.0f, true);
    }
}

void AMainCube::Interaction()
{
    FVector LocationToCheck = GetActorLocation();
    FVector TraceBoxSize = FVector(60.0f, 60.0f, 55.0f);
    FRotator Orientation = GetActorRotation();

    FHitResult HitResult;
    bool bHit = UKismetSystemLibrary::BoxTraceSingle(
        this,
        LocationToCheck,
        LocationToCheck,
        TraceBoxSize,
        Orientation,
        UEngineTypes::ConvertToTraceType(ECC_Visibility),
        false,
        IgnoredActors,
        EDrawDebugTrace::ForDuration,
        HitResult,
        true
    );

    if (bHit && HitResult.GetActor())
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor->GetClass()->ImplementsInterface(UInteraction::StaticClass()))
        {
            IInteraction* Interaction = Cast<IInteraction>(HitActor);
            Interaction->Execute_Interact(HitActor);
        }
    }
}