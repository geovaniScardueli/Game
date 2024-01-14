// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

void AEnemyAIController::DisableBehaviorTree()
{
	GetBrainComponent()->StopLogic("Die");
	GetPerceptionComponent()->SetSenseEnabled(sightConfig->GetSenseImplementation(), false);
}