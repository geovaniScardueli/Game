// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoMortoNotify.h"

#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/InimigoPadraoAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/InimigoPadrao1.h"
#include "PrimeiroGame/Personagens/Enemy/Archer/Archer.h"
#include "PrimeiroGame/Personagens/Enemy/Archer/ArcherAnimInstance.h"

void UInimigoMortoNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AInimigoPadrao1* InimigoPadrao = Cast<AInimigoPadrao1>(MeshComp->GetOwner()))
	{
		InimigoPadrao->PhysicsWeapon();
		Cast<UInimigoPadraoAnimInstance>(InimigoPadrao->GetMesh()->GetAnimInstance())->CharacterIsDead();
		InimigoPadrao->GetMesh()->SetSimulatePhysics(true);
		InimigoPadrao->GetPlayer()->VerifyEnemyLockIsDead();
	}
	else if (AArcher* Archer = Cast<AArcher>(MeshComp->GetOwner()))
	{
		Cast<UArcherAnimInstance>(Archer->GetMesh()->GetAnimInstance())->CharacterIsDead();
		Archer->GetMesh()->SetSimulatePhysics(true);
		Archer->GetPlayer()->VerifyEnemyLockIsDead();
	}
}
