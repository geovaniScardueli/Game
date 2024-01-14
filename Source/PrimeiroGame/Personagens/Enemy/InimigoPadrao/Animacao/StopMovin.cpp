// Fill out your copyright notice in the Description page of Project Settings.


#include "StopMovin.h"

#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/InimigoPadrao1.h"

void UStopMovin::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AInimigoPadrao1* Ator = Cast<AInimigoPadrao1>(MeshComp->GetOwner());
	if (Ator)
	{
		Ator->ChangeBlackboarValue("EstaAtacando", false);
		Ator->LaunchCharacter(Ator->GetActorForwardVector() * 700.f, true, true);
	}
}
