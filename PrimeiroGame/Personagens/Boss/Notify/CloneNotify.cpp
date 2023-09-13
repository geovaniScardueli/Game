// Fill out your copyright notice in the Description page of Project Settings.


#include "CloneNotify.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyraClone.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"
#include "Kismet/GameplayStatics.h"

void UCloneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABossNyraClone* Boss = Cast<ABossNyraClone>(MeshComp->GetOwner()))
	{
		if (IdSection == "CloneKick")
		{
			Cast<AProtagonista>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->LaunchPlayer(
				FVector(Boss->GetActorForwardVector().X * 5700.f,
				Boss->GetActorForwardVector().Y * 5700.f,
				7000.f));
		}
	}
}