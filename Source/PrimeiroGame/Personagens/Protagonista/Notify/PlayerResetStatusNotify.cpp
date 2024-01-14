// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerResetStatusNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerResetStatusNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Character->ResetStatus(IdSection);
	}
}
