// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChaserCharacter::AChaserCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PatrolRadius = 500.0f;
    DetectionRadius = 1000.0f;
    CurrentState = EChaserState::Patrol;
}

// Called when the game starts or when spawned
void AChaserCharacter::BeginPlay()
{
	Super::BeginPlay();
    Patrol();
	
}

// Called every frame
void AChaserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    switch (CurrentState)
    {
    case EChaserState::Patrol:
        Patrol();
        if (DetectRunner())
        {
            SetState(EChaserState::Chase);
        }
        break;

    case EChaserState::Chase:
        if (DetectedRunner)
        {
            Chase(DetectedRunner);
            if (FVector::Dist(GetActorLocation(), DetectedRunner->GetActorLocation()) > DetectionRadius)
            {
                SetState(EChaserState::Patrol);
            }
        }
        else
        {
            SetState(EChaserState::Patrol);
        }
        break;

    default:
        break;
    }
}

// Called to bind functionality to input
void AChaserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChaserCharacter::Patrol()
{
    if (PatrolPoints.Num() > 0)
    {
        CurrentPatrolPoint = PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)];
        if (CurrentPatrolPoint)
        {
            FVector Direction = (CurrentPatrolPoint->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            AddMovementInput(Direction);
        }
    }
}

void AChaserCharacter::Chase(AActor* Runner)
{
    FVector Direction = (Runner->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction);
}

bool AChaserCharacter::DetectRunner()
{
    AActor* Runner = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assuming runner is the player
    if (Runner)
    {
        float DistanceToRunner = FVector::Dist(GetActorLocation(), Runner->GetActorLocation());
        if (DistanceToRunner <= DetectionRadius)
        {
            DetectedRunner = Runner;
            return true;
        }
    }
    DetectedRunner = nullptr;
    return false;
}

void AChaserCharacter::SetState(EChaserState NewState)
{
    CurrentState = NewState;
    if (NewState == EChaserState::Patrol)
    {
        CurrentPatrolPoint = nullptr;
    }
}

