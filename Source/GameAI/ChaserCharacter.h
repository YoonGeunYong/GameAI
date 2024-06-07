// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChaserCharacter.generated.h"

UENUM(BlueprintType)
enum class EChaserState : uint8
{
	Patrol     UMETA(DisplayName = "Patrol"),
	Chase      UMETA(DisplayName = "Chase")
};

UCLASS()
class GAMEAI_API AChaserCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChaserCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AActor*> PatrolPoints;

private:
	EChaserState CurrentState;
	AActor* CurrentPatrolPoint;
	AActor* DetectedRunner;

	void Patrol();
	void Chase(AActor* Runner);
	bool DetectRunner();
	void SetState(EChaserState NewState);
};
