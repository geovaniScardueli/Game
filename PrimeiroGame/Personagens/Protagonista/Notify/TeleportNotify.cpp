// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportNotify.h"

#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UTeleportNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Character = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Character->MoveCamera();
	}
}
