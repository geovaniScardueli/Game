// Fill out your copyright notice in the Description page of Project Settings.


#include "SeguirPlayer.h"

#include <concrt.h>

#include "InimigoPadraoAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PrimeiroGame/Personagens/Inimigo.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadraoAnimInstance.h"

USeguirPlayer::USeguirPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = "Seguir Player";
}

EBTNodeResult::Type USeguirPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const cont = Cast<AInimigoPadraoAIController>(OwnerComp.GetAIOwner());
	//FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);
	
	//cont->GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	AInimigo* Inimigo = Cast<AInimigo>(cont->GetPawn());
	cont->GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), Inimigo->GetPlayer());
	float distance = FVector::Dist(Inimigo->GetActorLocation(), Inimigo->GetPlayer()->GetActorLocation());

	if (cont->GetBlackboardComponent()->GetValueAsBool(TEXT("AtacarPlayer")))
	{

		Cast<UInimigoPadraoAnimInstance>(Inimigo->GetMesh()->GetAnimInstance())->SetClosePlayer(false);
		if (distance < 200.f)
		{
			cont->GetBlackboardComponent()->SetValueAsBool(TEXT("IsRangeAtack"), true);
		}
	}
	else if (distance < 500.f)
	{
		Cast<UInimigoPadraoAnimInstance>(Inimigo->GetMesh()->GetAnimInstance())->SetClosePlayer(true);
		cont->GetBlackboardComponent()->SetValueAsBool(TEXT("ClosePlayer"), true);
	}
	else
	{
		Cast<UInimigoPadraoAnimInstance>(Inimigo->GetMesh()->GetAnimInstance())->SetClosePlayer(false);
	}
	//cont->GetBlackboardComponent()->SetValueAsVector(TEXT("VaiPara"), Inimigo->GetPlayer()->GetActorLocation());
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
	//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//return EBTNodeResult::Succeeded;
}
