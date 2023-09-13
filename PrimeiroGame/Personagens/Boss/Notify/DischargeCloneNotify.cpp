// Fill out your copyright notice in the Description page of Project Settings.


#include "DischargeCloneNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"

void UDischargeCloneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//EventReference->GetContextData();
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		if (IdSection == "DisClone")
		{
			Boss->CreateClone();
		}
		else if (IdSection == "Kick")
		{
			Boss->CreateCloneKick();
		}
	}
}