// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyResetAllStatusNotify.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

void UEnemyResetAllStatusNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Enemy = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Enemy->ResetAllStatus();
	}
}
