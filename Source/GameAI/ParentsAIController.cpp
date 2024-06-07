// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentsAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameAIGameMode.h"
#include "AIController.h"

void AParentsAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AParentsAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	MoveToActor(PlayerPawn);

}