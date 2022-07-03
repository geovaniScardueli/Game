// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoMortoNotify.h"

#include "PrimeiroGame/Personagens/Inimigo.h"

void UInimigoMortoNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Character = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Character->Destroy();
	}
}
