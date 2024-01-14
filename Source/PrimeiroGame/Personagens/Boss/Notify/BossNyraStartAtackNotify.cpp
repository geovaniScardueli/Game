// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNyraStartAtackNotify.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

void UBossNyraStartAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Enemy = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Enemy->StartAttack();
	}
}