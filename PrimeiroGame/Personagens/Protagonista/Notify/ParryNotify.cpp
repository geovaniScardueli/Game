// Fill out your copyright notice in the Description page of Project Settings.


#include "ParryNotify.h"

#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UParryNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Character->bIsInParryAnim = false;
		Character->bIsInParryTime = false;
		//Character->ChangeMovementStatus(true);
		//Character->ChangeRotator(true);
		Character->OnEnterDefensePosition();
	}
}
