// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtagonistaAninInstance.h"

#include "Protagonista.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UProtagonistaAninInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = TryGetPawnOwner();
}

void UProtagonistaAninInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//precisa dessa linha, se não da erro na linha de baixo, provavel por q está no editor ae não tem as variáveis setadas
	if (!Owner) return;
	
	if (Owner->IsA(AProtagonista::StaticClass()))
	{
		AProtagonista* PlayerCharacter = Cast<AProtagonista>(Owner);
		if (PlayerCharacter)
		{
			bIsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
		}
	}
	//pegar a velocidade do personamge
	Velocidade = GetOwningActor()->GetVelocity().Size();
	Rotacao = UKismetAnimationLibrary::CalculateDirection(GetOwningActor()->GetVelocity(), GetOwningActor()->GetActorRotation());
	GoToRunState = Velocidade > 0.f;
	//Cast<AProtagonista>(GetOwningActor())->GetCharacterMovement();
}
