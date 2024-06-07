// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"

UENUM(BlueprintType)
enum class ERunnerState : uint8
{
	Idle      UMETA(DisplayName = "Idle"),
	RunAway   UMETA(DisplayName = "RunAway")
};

UCLASS()
class GAMEAI_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float EscapeDistance;

private:
	ERunnerState CurrentState;
	void SetState(ERunnerState NewState);

	void Escape();
};
