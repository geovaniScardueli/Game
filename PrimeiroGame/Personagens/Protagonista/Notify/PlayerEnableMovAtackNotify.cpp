// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEnableMovAtackNotify.h"

#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerEnableMovAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Player->ResetAllStatus();
	}
}
