// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetStaminaNotify.h"

#include "Inimigo.h"
#include "Protagonista/Protagonista.h"

void UResetStaminaNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("Chegou no reset stamina bla"));
	if (AInimigo* Character = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Chegou no reset stamina"));
		Character->ChangeStamina(-25.f);
		Character->ChangeExecuteMode(0.f);
		Character->ChangeBlackboarValue("CanMov", true);
		Character->ChangeBlackboarValue("AtacarPlayer", false);
	} else if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Player->ResetPerfectPArry();
	}
}
