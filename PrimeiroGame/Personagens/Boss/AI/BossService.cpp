// Fill out your copyright notice in the Description page of Project Settings.


#include "BossService.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"
#include "PrimeiroGame/Personagens/Boss/AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"
#include "Kismet/KismetMathLibrary.h"

UBossService::UBossService(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Boss Service";

	bNotifyBecomeRelevant = true;	// necessarily!!!

	Interval = 0.5f;	// Any value.
	RandomDeviation = 0.f;
}

void UBossService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const Cont = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	ABossNyra* Boss = Cast<ABossNyra>(Cont->GetPawn());
	const float Distance = FVector::Dist(Boss->GetActorLocation(), Boss->GetPlayer()->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Distancia %f"), Distance);
	
	const int16 Percent = UKismetMathLibrary::RandomIntegerInRange(1, 100);
	if (Distance > 800.f && Percent > 30)
	{
		Boss->DischargeSingleClone();
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("CantMove"), false);
		return;
	}
	if (Distance > 400.f && Percent > 50)
	{
		Boss->StartTeleport();
		return;
	}
	if (Distance > 300.f)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("MoveToPLayer"), true);
		return;
	}

	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtackPlayer"), true);
	
}
