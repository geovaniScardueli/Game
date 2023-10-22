// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskBossAtackPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"
#include "PrimeiroGame/Personagens/Boss/AI/BossAIController.h"

UTaskBossAtackPlayer::UTaskBossAtackPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = "Atacar Player";
}

EBTNodeResult::Type UTaskBossAtackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* Cont = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = Cont->GetPawn();
	Cast<ABossNyra>(Pawn)->AtackPlayer();

	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("CanMov"), false);
	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtackPlayer"), false);
	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("MoveToPLayer"), false);
	

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}