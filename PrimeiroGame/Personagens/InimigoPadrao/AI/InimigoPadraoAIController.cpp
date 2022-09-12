// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoPadraoAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadrao1.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadraoAnimInstance.h"

AInimigoPadraoAIController::AInimigoPadraoAIController()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component")));
	
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	sightConfig->SightRadius = 600.f;
	sightConfig->LoseSightRadius = 1000.0f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	GetAIPerceptionComponent()->ConfigureSense(*sightConfig);
	GetAIPerceptionComponent()->SetDominantSense(sightConfig->GetSenseImplementation());
}

void AInimigoPadraoAIController::DisableBehaviorTree()
{
	GetBrainComponent()->StopLogic("Die");
	GetPerceptionComponent()->SetSenseEnabled(sightConfig->GetSenseImplementation(),false);
}

void AInimigoPadraoAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	AInimigoPadrao1* InimmigoPadrao = Cast<AInimigoPadrao1>(GetPawn());
	if (InimmigoPadrao)
	{
		//InimmigoPadrao->TesteAiController();
	}
}

void AInimigoPadraoAIController::BeginPlay()
{
	Super::BeginPlay();
	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AInimigoPadraoAIController::PercebeuAtores3);
}

void AInimigoPadraoAIController::PercebeuAtores3(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->ActorHasTag("InimigoPadrao")) return;

	AInimigo* Inimigo = Cast<AInimigo>(GetPawn());
	if (Stimulus.WasSuccessfullySensed())
	{
		Inimigo->SensePlayer();
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
		GetBlackboardComponent()->SetValueAsBool(TEXT("ViuPlayer"), false);
		Inimigo->SetSeePlayer(false);
		Inimigo->ChangeVisibilityUI(false);
		Inimigo->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("I see %s"),*Stimulus.Type.Name.ToString()));
}
