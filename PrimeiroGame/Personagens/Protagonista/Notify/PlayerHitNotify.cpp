// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHitNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Character->EnableDisableOverlapMesh(true);
		Character->ResetIndexAtackSequence();
	}
}
