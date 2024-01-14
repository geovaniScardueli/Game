// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportEndNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UTeleportEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		Player->GetMesh()->SetVisibility(true);
		Player->DestroyActorSword();
	}
}