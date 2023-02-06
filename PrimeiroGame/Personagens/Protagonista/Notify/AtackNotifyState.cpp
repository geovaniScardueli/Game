// Fill out your copyright notice in the Description page of Project Settings.


#include "AtackNotifyState.h"

#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
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
		if (Character->GetCurrentMontage() && Character->GetCurrentMontage()->GetName().Equals("Atack_AM"))
		{
			Character->ChangeIndexAtackSequence();
		}
		Character->EnableDisabelOverlapWeapon(false);
		Character->ChangeAtackStatus(true);
		Character->ResetPerfectPArry();
	} else if (AInimigo* Inimigo = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Inimigo->EnableDisabelOverlapWeapon(false);
	}
}
