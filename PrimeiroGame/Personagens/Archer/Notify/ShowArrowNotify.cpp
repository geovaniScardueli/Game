// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowArrowNotify.h"

#include "PrimeiroGame/Personagens/Archer/Archer.h"

void UShowArrowNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AArcher* Character = Cast<AArcher>(MeshComp->GetOwner()))
	{
		Character->VisibilityArrow(true);
	}
}
