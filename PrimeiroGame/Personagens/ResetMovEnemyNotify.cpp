// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetMovEnemyNotify.h"

#include "Inimigo.h"
#include "InimigoPadrao/InimigoPadraoAnimInstance.h"

void UResetMovEnemyNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Character = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Character->GetCapsuleComponent()->SetGenerateOverlapEvents(true);
		Character->ChangeBlackboarValue("CanMov", true);
		Character->ChangeBlackboarValue("AtacarPlayer", false);
		Character->ChangeBlackboarValue("ClosePlayer", true);
		Cast<UInimigoPadraoAnimInstance>(Character->GetMesh()->GetAnimInstance())->SetClosePlayer(true);
	}
}
