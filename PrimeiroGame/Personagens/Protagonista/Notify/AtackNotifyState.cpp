// Fill out your copyright notice in the Description page of Project Settings.


#include "AtackNotifyState.h"

#include "PrimeiroGame/Personagens/Inimigo.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UAtackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Character->EnableDisabelOverlapWeapon(true);
		Character->ChangeRotator(false);
	} else if (AInimigo* Inimigo = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Inimigo->EnableDisabelOverlapWeapon(true);
	}
}


void UAtackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Character->EnableDisabelOverlapWeapon(false);
		Character->ChangeIndexAtackSequence();
		Character->ChangeAtackStatus(true);
		Character->ResetPerfectPArry();
	} else if (AInimigo* Inimigo = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Inimigo->EnableDisabelOverlapWeapon(false);
	}
}
