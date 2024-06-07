// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EscapeDistance = 1500.0f;
	CurrentState = ERunnerState::Idle;

}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    switch (CurrentState)
    {
    case ERunnerState::Idle:
        // 대기 상태에서는 아무 것도 하지 않음
        break;

    case ERunnerState::RunAway:
        Escape();
        if (FVector::Dist(GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()) > EscapeDistance)
        {
            SetState(ERunnerState::Idle);
        }
        break;

    default:
        break;
    }

}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARunnerCharacter::Escape()
{
    FVector AwayDirection = (GetActorLocation() - UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()).GetSafeNormal();
    AddMovementInput(AwayDirection);
}

