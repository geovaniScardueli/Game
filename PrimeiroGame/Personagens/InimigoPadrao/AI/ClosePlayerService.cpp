// Fill out your copyright notice in the Description page of Project Settings.


#include "ClosePlayerService.h"

#include "InimigoPadraoAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/Personagens/Inimigo.h"


UClosePlayerService::UClosePlayerService(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Close Player";

	bNotifyBecomeRelevant = true;	// necessarily!!!

	Interval = 0.5f;	// Any value.
	RandomDeviation = 0.f;
}

void UClosePlayerService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	auto const Cont = Cast<AInimigoPadraoAIController>(OwnerComp.GetAIOwner());
	AInimigo* Inimigo = Cast<AInimigo>(Cont->GetPawn());
	float Distance = FVector::Dist(Inimigo->GetActorLocation(), Inimigo->GetPlayer()->GetActorLocation());
	if (Distance > 500.f)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("ClosePlayer"), false);
		return;
	}

	if (UKismetMathLibrary::RandomInteger(2) == 0)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), true);
	}

	//UBlackboardComponent* CurretnBlackboard = OwnerComp.GetBlackboardComponent();
//
	//if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())
	//{
	//	AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	//}

}
