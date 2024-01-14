// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStartAttackNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerStartAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Player->StartAttack();
	}
}
