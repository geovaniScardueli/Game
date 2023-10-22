// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStopAttackNotify.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

void UEnemyStopAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Enemy = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Enemy->StopAttack();
	}
}
