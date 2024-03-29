// Fill out your copyright notice in the Description page of Project Settings.


#include "NyraAnimInstance.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"

void UNyraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//pegar a velocidade do personamge
	Velocidade = GetOwningActor()->GetVelocity().Size();
	//GoToRunState = Velocidade > 0.f;
	Rotacao = UKismetAnimationLibrary::CalculateDirection(GetOwningActor()->GetVelocity(), GetOwningActor()->GetActorRotation());
}