// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMovePlayerTask.h"
#include "PrimeiroGame/Personagens/Boss/AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/InimigoPadrao1.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

UBossMovePlayerTask::UBossMovePlayerTask(FObjectInitializer const& object_initializer)
{
	NodeName = "Boss Move Player Task";
}

EBTNodeResult::Type UBossMovePlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* Cont = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	//APawn* Pawn = Cont->GetPawn();
	//Cast<ABossNyra>(Pawn)->AtackPlayer();
	//Cast<ABossNyra>(Pawn)->ChangeBlackboarValue("CanMov", false);

	//Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
	//Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("IsRangeAtack"), false);
	ABossNyra* Boss = Cast<ABossNyra>(Cont->GetPawn());
	Cont->GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), Boss->GetPlayer());
	Boss->ChangeBlackboarValue("MoveToPLayer", false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}