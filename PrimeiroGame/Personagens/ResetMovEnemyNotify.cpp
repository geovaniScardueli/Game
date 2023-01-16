// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetMovEnemyNotify.h"

#include "Inimigo.h"
#include "PrimeiroGame/Personagens/EnemyAnimInstance.h"

void UResetMovEnemyNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Character = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
		Character->ChangeBlackboarValue("CanMov", true);
		Character->ChangeBlackboarValue("AtacarPlayer", false);
		Character->ChangeBlackboarValue("ViuPlayer", true);
		Character->ChangeBlackboarValue("ClosePlayer", true);
		Character->ChangeBlackboarValue("SpecialAtack", false);
		Cast<UEnemyAnimInstance>(Character->GetMesh()->GetAnimInstance())->SetClosePlayer(true);
	}
}
