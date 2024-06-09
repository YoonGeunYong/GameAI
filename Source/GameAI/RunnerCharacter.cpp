// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "RunnerAIController.h"

ARunnerCharacter::ARunnerCharacter()
{
	AIControllerClass = ARunnerAIController::StaticClass();
}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}