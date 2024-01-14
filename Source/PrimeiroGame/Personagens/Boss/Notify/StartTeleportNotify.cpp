// Fill out your copyright notice in the Description page of Project Settings.


#include "StartTeleportNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"

void UStartTeleportNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		Boss->CheckToTeleport();
	}
}
