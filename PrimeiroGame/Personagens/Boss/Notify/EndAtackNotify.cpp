// Fill out your copyright notice in the Description page of Project Settings.

#include "EndAtackNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"

void UEndAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		Boss->EnableDisableOverlapWeapon1(false);
	}
}
