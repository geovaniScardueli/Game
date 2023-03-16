// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAtack.h"
#include "PrimeiroGame/Personagens/Boss/AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/InimigoPadrao1.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"

UBossAtack::UBossAtack(FObjectInitializer const& object_initializer)
{
	NodeName = "Boss Atack";
}

EBTNodeResult::Type UBossAtack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* Cont = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = Cont->GetPawn();
	ABossNyra* Boss = Cast<ABossNyra>(Pawn);
	Boss->AtackPlayer();
	
	//Cast<ABossNyra>(Pawn)->ChangeBlackboarValue("CanMov", false);

	//Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
	//Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("IsRangeAtack"), false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}