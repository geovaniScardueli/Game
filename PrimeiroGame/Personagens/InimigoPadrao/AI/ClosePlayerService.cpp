// Fill out your copyright notice in the Description page of Project Settings.


#include "ClosePlayerService.h"

#include "InimigoPadraoAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/Personagens/Inimigo.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"


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
	
	//TArray<FHitResult> OutHits;
	//
	//// start and end locations
	//FVector SweepStart = Inimigo->GetActorLocation();
	//FVector SweepEnd = Inimigo->GetActorLocation();
	//
	//// create a collision sphere
	//FCollisionShape MyColSphere = FCollisionShape::MakeSphere(100.0f);
	//
	//// draw collision sphere
	//DrawDebugSphere(GetWorld(), SweepStart, MyColSphere.GetSphereRadius(), 10, FColor::Purple, true);
	//
	//// check if something got hit in the sweep
	//bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyColSphere);
	//
	//if (isHit)
	//{
	//	// loop through TArray
	//	for (auto& Hit : OutHits)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("resultado: %s"), *Hit.GetActor()->GetName());
	//		if (GEngine)
	//		{
	//			
	//			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
	//		}
	//	}
	//}

	

	if (Distance > 500.f)
	{
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("AtacarPlayer"), false);
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("ClosePlayer"), false);
		return;
	}

	int Temp = UKismetMathLibrary::RandomInteger(10);

	if (Temp < 5)
	{
		TArray<FHitResult> HitArray;
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(Inimigo);
		bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Inimigo->GetActorLocation(), Inimigo->GetActorLocation(), 500,
			UEngineTypes::ConvertToTraceType(ECC_Pawn), false, ActorToIgnore, EDrawDebugTrace::None, HitArray, true,
			FLinearColor::Gray, FLinearColor::Blue, 0.f);

		if (Hit)
		{
			AInimigo* Other = nullptr;
			for (const FHitResult& HitResult : HitArray)
			{
				if (HitResult.GetActor()->ActorHasTag("InimigoPadrao"))
				{
					Other = Cast<AInimigo>(HitResult.GetActor());
					Other->SensePlayer();
					break;
				}
			}
			if (Other)
			{
				APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
				FVector WordLocation = Player->GetActorLocation();
				FVector DirecaoLeft = (Player->GetActorRightVector() * 150.f * -1) + WordLocation;
				FVector DirecaoRight = (Player->GetActorRightVector() * 150.f) + WordLocation;

				UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
				UNavigationPath* PathLeft = navSys->FindPathToLocationSynchronously(Other, Other->GetActorLocation(), DirecaoLeft);
				UNavigationPath* PathRight = navSys->FindPathToLocationSynchronously(Other, Other->GetActorLocation(), DirecaoRight);
				if (PathLeft && PathRight && PathLeft->PathPoints.Num() > 0 && PathRight->PathPoints.Num() > 0)
				{
					Other->SpecialAtack(DirecaoLeft);
					Inimigo->SpecialAtack(DirecaoRight);
				}
			}
		}
		
	}

	if (Temp < 8)
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

