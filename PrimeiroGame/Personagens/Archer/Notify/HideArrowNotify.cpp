// Fill out your copyright notice in the Description page of Project Settings.


#include "HideArrowNotify.h"

#include "PrimeiroGame/Personagens/Archer/Archer.h"

void UHideArrowNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AArcher* Character = Cast<AArcher>(MeshComp->GetOwner()))
	{
		Character->VisibilityArrow(false);
	}
}
