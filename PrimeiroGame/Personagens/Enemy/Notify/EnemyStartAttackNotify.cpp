// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStartAttackNotify.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

void UEnemyStartAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Enemy = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Enemy->StartAttack();
	}
}