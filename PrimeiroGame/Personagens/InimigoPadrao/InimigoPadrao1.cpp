// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoPadrao1.h"

#include "AI/InimigoPadraoAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AInimigoPadrao1::AInimigoPadrao1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

// Called when the game starts or when spawned
void AInimigoPadrao1::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInimigoPadrao1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInimigoPadrao1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInimigoPadrao1::TesteAiController()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("entrou no ai controleer no inimigo"));
	if (bAndou)
	{
		Cast<AInimigoPadraoAIController>(GetController())->MoveToLocation(FVector(740.f, -490.f,598.f));
	} else
	{
		Cast<AInimigoPadraoAIController>(GetController())->MoveToLocation(FVector(740.f, 320.f,598.f));
	}
	bAndou = !bAndou;
}

