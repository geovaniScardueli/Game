// Fill out your copyright notice in the Description page of Project Settings.


#include "AtackPlayer.h"
#include "PrimeiroGame/Personagens/AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Inimigo.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadrao1.h"

UAtackPlayer::UAtackPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = "Atacar Player";
}

EBTNodeResult::Type UAtackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = Cont->GetPawn();
	Cast<AInimigo>(Pawn)->AtackPlayer();
	Cast<AInimigo>(Pawn)->ChangeBlackboarValue("CanMov", false);
	
	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("IsRangeAtack"), false);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
