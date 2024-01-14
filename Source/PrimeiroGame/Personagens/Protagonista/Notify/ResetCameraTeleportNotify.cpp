// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetCameraTeleportNotify.h"

#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UResetCameraTeleportNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		//Character->EnabledInTeleportMoviment();
	}
}
