// Fill out your copyright notice in the Description page of Project Settings.

#include "PrimeiroGame.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Modules/ModuleManager.h"
#include "Personagens/Protagonista/Protagonista.h"
#include "Personagens/Protagonista/UI/PlayerWidget.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, PrimeiroGame, "PrimeiroGame");

void APrimeiroGame::MoverLocalMovimentacao()
{
	if (!LocalMovimentar.IsZero())
	{
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->SetActorLocation(LocalMovimentar);
	}
}

void APrimeiroGame::BeginPlay()
{
	GameHud = Cast<UPlayerWidget>(CreateWidget(GetWorld(), GameHudClass));
	if (IsValid(GameHud))
	{
		GameHud->AddToViewport();	
	}
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

