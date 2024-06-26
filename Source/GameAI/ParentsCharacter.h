// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParentsCharacter.generated.h"

UCLASS()
class GAMEAI_API AParentsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AParentsCharacter();

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
	AActor* CurrentPatrolPoint;
	AActor* DetectedRunner;

	void Patrol();
	void Chase(AActor* Runner);
	bool DetectRunner();
};
