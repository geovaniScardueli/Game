// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

AInimigoAIController::AInimigoAIController()
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

void AInimigoAIController::BeginPlay()
{
	Super::BeginPlay();
	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AInimigoAIController::PercebeuAtores3);
}

void AInimigoAIController::PercebeuAtores3(AActor* Actor, FAIStimulus Stimulus)
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
}
