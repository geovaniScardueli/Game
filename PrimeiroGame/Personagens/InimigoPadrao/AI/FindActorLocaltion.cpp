// Fill out your copyright notice in the Description page of Project Settings.


#include "FindActorLocaltion.h"

#include "InimigoPadraoAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/Personagens/Inimigo.h"

UFindActorLocaltion::UFindActorLocaltion(FObjectInitializer const& object_initializer)
{
	NodeName = "Encontrar Localizcao Aleatoria";
}

EBTNodeResult::Type UFindActorLocaltion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const cont = Cast<AInimigoPadraoAIController>(OwnerComp.GetAIOwner());
	AInimigoPadraoAIController* Cont = Cast<AInimigoPadraoAIController>(OwnerComp.GetAIOwner());
	AInimigo* Inimigo = Cast<AInimigo>(Cont->GetPawn());
	
	//Inimigo->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Inimigo->GetActorLocation(),
	//	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()));
	FVector WordLocation = Inimigo->GetActorLocation();
	FVector Direcao;
	
	int valor = UKismetMathLibrary::RandomInteger(3);
	const float Distance = 150.f;
	switch (valor)
	{
	case 1:
		Direcao = Inimigo->GetActorRightVector() * Distance;
		break;
	case 2:
		Direcao = Inimigo->GetActorRightVector() * Distance * -1;
		break;
	case 0:
	default:
		Direcao = Inimigo->GetActorForwardVector() * Distance * -1;
		break;
	}
	
	cont->GetBlackboardComponent()->SetValueAsVector(TEXT("VaiPara"), WordLocation + Direcao);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
