// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserCharacter.h"
#include "ChaserAIController.h"

AChaserCharacter::AChaserCharacter()
{
	AIControllerClass = AChaserAIController::StaticClass();
}

// Called to bind functionality to input
void AChaserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}