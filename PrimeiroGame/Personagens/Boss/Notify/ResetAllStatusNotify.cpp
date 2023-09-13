// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetAllStatusNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"

void UResetAllStatusNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		Boss->ResetAllStatus();
	}
}