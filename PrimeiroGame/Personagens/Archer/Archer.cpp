// Fill out your copyright notice in the Description page of Project Settings.


#include "Archer.h"
#include "AI/ArcherAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/Personagens/Archer/Arrow/Arrow.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"


void AArcher::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->HideBoneByName(FName("DEF_Arrow"), EPhysBodyOp::PBO_None);
	AController* temp = GetController();
	ArcherAIController = Cast<AArcherAIController>(temp);
	ChangeBlackboarValue("CanMov", true);
	//VisibilityArrow(false);
}

void AArcher::ChangeBlackboarValue(const FName Description, bool Val)
{
	if (Description.ToString().Equals("CanMov"))
	{
		bCanMove = Val;
	}
	if (ArcherAIController)
		ArcherAIController->GetBlackboardComponent()->SetValueAsBool(Description, Val);
}

void AArcher::AtackPlayer()
{
	PlayAnimMontage(Montages[EEnemyMontages::EEAtack], 1, FName("AtackPlayer"));
}

void AArcher::VisibilityArrow(const bool bShow)
{
	if (bShow)
	{
		GetMesh()->UnHideBoneByName(FName("DEF_Arrow"));
	}
	else
	{
		GetMesh()->HideBoneByName(FName("DEF_Arrow"), EPhysBodyOp::PBO_None);
		ShootArrow();
	}
}

void AArcher::ShootArrow()
{
	const FRotator Rota = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName("DEF_Arrow")),
		GameMode->GetPlayer()->GetMesh()->GetSocketLocation(FName("DEF-spine_003")));
	Arrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, GetMesh()->GetSocketLocation(FName("DEF_Arrow")), Rota);
}

void AArcher::SensePlayer()
{
	SetSeePlayer(true);
	ChangeBlackboarValue(TEXT("SpecialAtack"), false);
	ChangeBlackboarValue(TEXT("ViuPlayer"), true);
	ChangeBlackboarValue(TEXT("AtacarPlayer"), true);
}
