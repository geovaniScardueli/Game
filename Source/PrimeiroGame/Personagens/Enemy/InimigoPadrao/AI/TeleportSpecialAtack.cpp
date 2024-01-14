// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportSpecialAtack.h"

#include "Kismet/GameplayStatics.h"
#include "PrimeiroGame/Personagens/Enemy/AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
#include "GameFramework/CharacterMovementComponent.h"

UTeleportSpecialAtack::UTeleportSpecialAtack(FObjectInitializer const& object_initializer)
{
	NodeName = "Teleport Special";
}

EBTNodeResult::Type UTeleportSpecialAtack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player)
	{
		auto const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
		
		FVector WordLocation = Player->GetActorLocation();
		FVector Direcao;
		Direcao = (Player->GetActorRightVector() * 150.f * -1) + WordLocation;
		//cont->GetBlackboardComponent()->SetValueAsVector(TEXT("VaiPara"), Direcao);
		//
		UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
		UNavigationPath* path = navSys->FindPathToLocationSynchronously(cont->GetPawn(), cont->GetPawn()->GetActorLocation(), Direcao);
		if (path->PathPoints.Num() > 0)
		{
			Cast<AInimigo>(cont->GetPawn())->SetActorLocation(Direcao);
		}
		
		//DrawDebugSphere(GetWorld(), Direcao, 50.f, 3, FColor::Red);
	
		
		
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}