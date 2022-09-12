// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoMortoNotify.h"

#include "PrimeiroGame/Personagens/Inimigo.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadraoAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UInimigoMortoNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigo* Character = Cast<AInimigo>(MeshComp->GetOwner()))
	{
		Cast<UInimigoPadraoAnimInstance>(Character->GetMesh()->GetAnimInstance())->CharacterIsDead();
		Character->GetMesh()->SetSimulatePhysics(true);
		Cast<AProtagonista>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->VerifyEnemyLockIsDead();
	}
}
