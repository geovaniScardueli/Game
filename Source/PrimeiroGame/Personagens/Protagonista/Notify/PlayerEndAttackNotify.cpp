// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEndAttackNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerEndAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		//if (Player->GetCurrentMontage() && Player->GetCurrentMontage()->GetName().Equals("AM_Atack"))
		//{
		//	Player->ChangeIndexAtackSequence();
		//}
		Player->StopAttack();
	}
}
