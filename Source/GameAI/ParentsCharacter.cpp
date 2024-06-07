// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentsCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AParentsCharacter::AParentsCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PatrolRadius = 500.0f;
	DetectionRadius = 1000.0f;

}

// Called when the game starts or when spawned
void AParentsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParentsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (DetectedRunner)
    {
        Chase(DetectedRunner);
    }
    else
    {
        if (DetectRunner())
        {
            DetectedRunner = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assuming runner is the player
        }
        else
        {
            Patrol();
        }
    }

}

// Called to bind functionality to input
void AParentsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AParentsCharacter::Patrol()
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

void AParentsCharacter::Chase(AActor* Runner)
{
    FVector Direction = (Runner->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction);
}

bool AParentsCharacter::DetectRunner()
{
    AActor* Runner = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Assuming runner is the player
    if (Runner)
    {
        float DistanceToRunner = FVector::Dist(GetActorLocation(), Runner->GetActorLocation());
        return DistanceToRunner <= DetectionRadius;
    }
    return false;
}

