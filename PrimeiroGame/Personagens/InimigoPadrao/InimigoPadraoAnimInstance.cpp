// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoPadraoAnimInstance.h"

#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PrimeiroGame/Personagens/Inimigo.h"

void UInimigoPadraoAnimInstance::CharacterIsDead()
{
	bIsdead = true; 
}

void UInimigoPadraoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//pegar a velocidade do personamge
	Velocidade = GetOwningActor()->GetVelocity().Size();
	Rotacao = UKismetAnimationLibrary::CalculateDirection(GetOwningActor()->GetVelocity(), GetOwningActor()->GetActorRotation());
	GoToRunState = Velocidade > 0.f;
}

void UInimigoPadraoAnimInstance::SetClosePlayer(bool Val)
{
	bCloseplayerSpace = Val;
	if (bCloseplayerSpace)
	{
		Cast<AInimigo>(GetOwningActor())->GetCharacterMovement()->MaxWalkSpeed = 100.f;
	} else {
		Cast<AInimigo>(GetOwningActor())->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}


