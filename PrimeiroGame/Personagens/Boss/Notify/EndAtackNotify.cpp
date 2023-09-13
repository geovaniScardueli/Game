// Fill out your copyright notice in the Description page of Project Settings.

#include "EndAtackNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UEndAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		Boss->EnableDisableOverlapWeapon1(false);
	}
	else if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		if (Player->GetCurrentMontage() && Player->GetCurrentMontage()->GetName().Equals("AM_Atack"))
		{
			Player->ChangeIndexAtackSequence();
		}
		Player->EnableDisabelOverlapWeapon(false);
		Player->ChangeAtackStatus(true);
		Player->ResetPerfectPArry();
	}
}
