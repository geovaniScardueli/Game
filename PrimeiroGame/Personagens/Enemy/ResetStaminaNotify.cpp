// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetStaminaNotify.h"

#include "Inimigo.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UResetStaminaNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Character = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Character->ChangeStamina(-25.f);
		Character->ChangeExecuteMode(0.f);
		Character->ChangeBlackboarValue("CanMov", true);
		Character->ChangeBlackboarValue("AtacarPlayer", false);
		if (Character->GetBlackboarValue(FName("ViuPlayer")))
		{
			Character->SetSeePlayer(true);
		}
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	} else if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Player->ResetPerfectPArry();
	}
}
