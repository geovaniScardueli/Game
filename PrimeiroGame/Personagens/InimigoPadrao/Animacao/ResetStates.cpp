// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetStates.h"

#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadrao1.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadraoAnimInstance.h"

void UResetStates::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AInimigoPadrao1* Ator = Cast<AInimigoPadrao1>(MeshComp->GetOwner());
	if (Ator)
	{
		Ator->ChangeBlackboarValue("ClosePlayer", false);
		Ator->ChangeBlackboarValue("CanMov", true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("finish atack")));
	}
}
