// Fill out your copyright notice in the Description page of Project Settings.


#include "LoseHeadNotify.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

void ULoseHeadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AInimigo* Enemy = Cast<AInimigo>(MeshComp->GetOwner());
	if (Enemy)
	{
		Enemy->Decapitated();

	}
}