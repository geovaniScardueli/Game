// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseAtackNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"

void UDefenseAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		Boss->CheckAfterParry();
	}
}
