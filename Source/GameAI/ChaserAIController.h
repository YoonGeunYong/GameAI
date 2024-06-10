// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ChaserAIController.generated.h"

class ARunnerAIController;

UENUM(BlueprintType)
enum class EChaserState : uint8
{
    Patrol      UMETA(DisplayName = "Patrol"),
    Chase       UMETA(DisplayName = "Chase"),
    Searching   UMETA(DisplayName = "Searching"),
    Catch       UMETA(DisplayName = "Catch")
};

UCLASS()
class GAMEAI_API AChaserAIController : public AAIController
{
	GENERATED_BODY()

public:
    AChaserAIController();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    EChaserState CurrentState;
    AActor* DetectedRunner;
    ARunnerAIController* RunnerAIController;
    bool CheckMove;
    bool End;
    float SearchTime;
    const float MaxSearchTime = 5.0f;

    FVector LastRunnerLocation;
    FVector PatrolStartLocation;
    FVector CurrentPatrolPoint;
    FVector GetRandomPatrolPoint();

    bool DetectRunner();
    void Patrol();
    void Chase(AActor* Runner);
    void Search();
    void SetState(EChaserState NewState);
};
