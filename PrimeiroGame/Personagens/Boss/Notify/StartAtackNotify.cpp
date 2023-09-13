// Fill out your copyright notice in the Description page of Project Settings.


#include "StartAtackNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UStartAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyra* Boss = Cast<ABossNyra>(MeshComp->GetOwner()))
	{
		if (Boss->GetMontageSection().IsEqual("Atack", ENameCase::CaseSensitive) || Boss->GetMontageSection().IsEqual("Atack5", ENameCase::CaseSensitive))
		{
			Boss->ChangeCapsuleWeapon("Weapon");
		}
		else if (Boss->GetMontageSection().IsEqual("Atack3", ENameCase::CaseSensitive))
		{
			Boss->ChangeCapsuleWeapon("Perna");
		}
		Boss->EnableDisableOverlapWeapon1(true);
	}
	else if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Player->EnableDisabelOverlapWeapon(true);
		Player->ChangeRotator(false);
	}
}