// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherAtackService.h"
#include "PrimeiroGame/Personagens/AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Inimigo.h"

UArcherAtackService::UArcherAtackService(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Atack Player";

	bNotifyBecomeRelevant = true;	// necessarily!!!

	Interval = 0.5f;	// Any value.
	RandomDeviation = 0.f;
}

void UArcherAtackService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	auto const Cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AInimigo* const Inimigo = Cast<AInimigo>(Cont->GetPawn());
	if (!Inimigo->IsSeePlayer())
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("ViuPlayer"), false);
		return;
	}
	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), true);
}