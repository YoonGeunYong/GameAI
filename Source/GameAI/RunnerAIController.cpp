// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "ChaserCharacter.h"

ARunnerAIController::ARunnerAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentState = ERunnerState::Idle;
    HideTime = 0.0f;
    Move = true;
    End = false;
}

void ARunnerAIController::BeginPlay()
{
    Super::BeginPlay();
    HideStartLocation = GetPawn()->GetActorLocation() - FVector(1100.0f, 1100.0f, 0.0f);
    HideEndLocation = GetPawn()->GetActorLocation() + FVector(1100.0f, 1100.0f, 0.0f);
    Chaser = UGameplayStatics::GetActorOfClass(GetWorld(), AChaserCharacter::StaticClass());
    SetState(ERunnerState::Idle);
}

void ARunnerAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (CurrentState)
    {
    case ERunnerState::Idle:
        // 대기 상태에서는 아무 것도 하지 않음
        break;

    case ERunnerState::RunAway:
        Escape();
        CheckDistanceToChaser();
        if (Dist > 300.0f)
        {
            SetState(ERunnerState::Hiding);
        }
        else if (Dist <= 150)
        {
            SetState(ERunnerState::Caught);
        }
        break;

    case ERunnerState::Hiding:
        if (Move)
            Hide();
        HideTime += DeltaTime;
        if (HideTime > MaxHideTime)
        {
            SetState(ERunnerState::Idle);
        }
        break;

    case ERunnerState::Caught:
        if (!End)
        {
            UE_LOG(LogTemp, Warning, TEXT("Catch Runner"));
            Move = false;
            End = true;
        }
    default:
        break;
    }
}

void ARunnerAIController::SetState(ERunnerState NewState)
{
    CurrentState = NewState;
    if (NewState == ERunnerState::Hiding)
    {
        HideTime = 0.0f;
    }
}

void ARunnerAIController::Escape()
{
    Move = true;

    CheckDistanceToChaser();
    if (Dist <= 350.0f)
    {
        // 도망갈 범위 설정        
        FBox AllowedArea(FVector(-1100.0f, -1100.0f, -300.0f), FVector(1100.0f, 1100.0f, 300.0f));

        // 추적자 방향 계산
        FVector AwayDirection = (GetPawn()->GetActorLocation() - Chaser->GetActorLocation()).GetSafeNormal();

        FVector DestinationOffset = (AllowedArea.GetCenter() - GetPawn()->GetActorLocation() + AwayDirection * 700.0f ).GetSafeNormal() * 700.0f;
        FVector DestinationLocation = GetPawn()->GetActorLocation() + DestinationOffset;
        
        // 계산된 도망 위치가 범위 내에 있는지 확인
        if (!AllowedArea.IsInside(DestinationLocation))
        {
            // 범위 밖이라면, 추적자 방향과 범위 경계 사이 지점으로 도망
            UE_LOG(LogTemp, Warning, TEXT("RunAway"));
            FVector RunawayDirection = FVector::CrossProduct(AwayDirection, AllowedArea.GetClosestPointTo(DestinationLocation) - GetPawn()->GetActorLocation()).GetSafeNormal();
            DestinationLocation = GetPawn()->GetActorLocation() + RunawayDirection * 700;
        }
        MoveToLocation(DestinationLocation, 5.0f);
    }
}

void ARunnerAIController::Hide()
{
    Move = false;
    FVector HidePoint = GetRandomHidePoint();
    MoveToLocation(HidePoint, 5.0f);
}

ERunnerState ARunnerAIController::GetState()
{
    return CurrentState;
}

FVector ARunnerAIController::GetRandomHidePoint()
{
    FVector HidePoint = FMath::RandPointInBox(FBox(FVector(-1100.0f, -1100.0f, 0.0f), FVector(1100.0f, 1100.0f, 0.0f)));
    return HidePoint;
}

void ARunnerAIController::CheckDistanceToChaser()
{
    Dist = FVector::Dist(GetPawn()->GetActorLocation(), Chaser->GetActorLocation());
}