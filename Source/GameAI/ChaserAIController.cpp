// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.h"
#include "RunnerAIController.h"

AChaserAIController::AChaserAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentState = EChaserState::Patrol;
    SearchTime = 0.0f;
    CheckMove = false;
    End = false;
}

void AChaserAIController::BeginPlay()
{
	Super::BeginPlay();
    PatrolStartLocation = GetPawn()->GetActorLocation();
    RunnerAIController = Cast<ARunnerAIController>(
        Cast<ACharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ARunnerCharacter::StaticClass()))->GetController());
    SetState(EChaserState::Patrol);
    UE_LOG(LogTemp, Warning, TEXT("[Chaser] StartState : Patrol"));
}

void AChaserAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    switch (CurrentState)
    {
    case EChaserState::Patrol:
        Patrol();
        if (DetectRunner())
        {
            SetState(EChaserState::Chase);
            UE_LOG(LogTemp, Warning, TEXT("[Chaser] Patrol -> Chase"));
        }
        break;

    case EChaserState::Chase:
        if (DetectedRunner)
        {
            Chase(DetectedRunner);
            float Distance = FVector::Dist(GetPawn()->GetActorLocation(), DetectedRunner->GetActorLocation());
            if (Distance > 300.0f)
            {
                LastRunnerLocation = DetectedRunner->GetActorLocation();
                SetState(EChaserState::Searching);
                UE_LOG(LogTemp, Warning, TEXT("[Chaser] Chase -> Searching -- Serching Around 5s"));
            }
            else if (Distance < 150.0f)
            {
                SetState(EChaserState::Catch);
                UE_LOG(LogTemp, Warning, TEXT("[Chaser] Chase -> Catch"));
            }
        }
        else
        {
            SetState(EChaserState::Searching);
            UE_LOG(LogTemp, Warning, TEXT("[Chaser] Chase -> Searching -- Serching Around 5s"));
        }
        break;

    case EChaserState::Searching:
        Search();
        SearchTime += DeltaTime;
        if (SearchTime > MaxSearchTime || DetectRunner())
        {
            SetState(EChaserState::Patrol);
            UE_LOG(LogTemp, Warning, TEXT("[Chaser] Searching -> Patrol"));
        }
        break;

    case EChaserState::Catch:
        if (!End)
        {
            UE_LOG(LogTemp, Warning, TEXT("Catch Runner"));
            End = true;
        }

    default:
        break;
    }
}

void AChaserAIController::Patrol()
{
    if (!CheckMove || FVector::Dist(GetPawn()->GetActorLocation(), CurrentPatrolPoint) < 100.0f)
    {
        CheckMove = true;
        CurrentPatrolPoint = GetRandomPatrolPoint();
        MoveToLocation(CurrentPatrolPoint, 5.0f);
    }
}

FVector AChaserAIController::GetRandomPatrolPoint()
{
    /*FVector CurrentLocation = GetPawn()->GetActorLocation();
    FVector PatrolPoint = CurrentLocation + FVector(FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f), 0.0f);*/

    return FVector(FMath::RandRange(-1100.0f, 1100.0f), FMath::RandRange(-1100.0f, 1100.0f), 0.0f);
}

void AChaserAIController::Chase(AActor* Runner)
{
    MoveToActor(Runner, 5.0f);
}


void AChaserAIController::Search()
{

    if (CheckMove && RunnerAIController->GetState() == ERunnerState::Hiding)
    {
        CheckMove = false;
        FVector SearchPoint = LastRunnerLocation + FMath::RandPointInBox(FBox(FVector(-400.0f, -400.0f, 0.0f), FVector(400.0f, 400.0f, 0.0f)));
        MoveToLocation(SearchPoint, 5.0f);
    }
}

bool AChaserAIController::DetectRunner()
{
    AActor* FoundRunner = UGameplayStatics::GetActorOfClass(GetWorld(), ARunnerCharacter::StaticClass());

    float DistanceToRunner = FVector::Dist(GetPawn()->GetActorLocation(), FoundRunner->GetActorLocation());
    if (DistanceToRunner <= 300.0f)
    {
        DetectedRunner = FoundRunner;
        RunnerAIController->SetState(ERunnerState::RunAway);
        UE_LOG(LogTemp, Warning, TEXT("[Runner] Idle,Hiding -> RunAway"));
        return true;
    }

    DetectedRunner = nullptr;
    return false;
}

void AChaserAIController::SetState(EChaserState NewState)
{
    CurrentState = NewState;
    if (NewState == EChaserState::Searching)
    {
        SearchTime = 0.0f;
    }
    else if (NewState == EChaserState::Patrol)
    {
        CheckMove = false;
    }
}
