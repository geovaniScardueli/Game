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
	//UE_LOG(LogTemp, Warning, TEXT("Distancia %f"), Distance);
	
	const int16 Percent = UKismetMathLibrary::RandomIntegerInRange(1, 100);
	//nao sei se isso vai funcionar
	//if (Distance > 400.f && Percent > 50)
	//{
	//	Boss->DischargeSingleClone();
	//	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("CanMov"), false);
	//	return;
	//}
	if (Distance > 400.f && Percent > 30)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("CanMov"), false);
		Boss->StartTeleport();
		return;
	}
	if (Distance > 300.f)
	{
		if (Percent > 60)
		{
			Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("MoveToPLayer"), true);
		}
		else
		{
			Boss->Grab();
		}
		return;
	}

	Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtackPlayer"), true);
	
}
