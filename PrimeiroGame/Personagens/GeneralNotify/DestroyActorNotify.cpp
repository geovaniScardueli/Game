// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyActorNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyraClone.h"

void UDestroyActorNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyraClone* Boss = Cast<ABossNyraClone>(MeshComp->GetOwner()))
	{
		Boss->DestroyClone();
	}
	
}