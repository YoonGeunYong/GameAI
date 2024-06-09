// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RunnerAIController.generated.h"

UENUM(BlueprintType)
enum class ERunnerState : uint8
{
    Idle       UMETA(DisplayName = "Idle"),
    RunAway    UMETA(DisplayName = "RunAway"),
    Hiding     UMETA(DisplayName = "Hiding"),
    Caught     UMETA(DisplayName = "Caught")
};

UCLASS()
class GAMEAI_API ARunnerAIController : public AAIController
{
	GENERATED_BODY()

public:
    ARunnerAIController();

    ERunnerState GetState();
    void SetState(ERunnerState NewState);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* Chaser;
    ERunnerState CurrentState;
    bool End;
    bool Move;
    float HideTime;
    float Dist;
    const float MaxHideTime = 10.0f;

    FVector HideStartLocation;
    FVector HideEndLocation;
    FVector GetRandomHidePoint();

    void Escape();
    void Hide();
    void CheckDistanceToChaser();

};
