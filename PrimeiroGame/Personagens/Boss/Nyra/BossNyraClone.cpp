// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNyraClone.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "PrimeiroGame/Personagens/Boss/PathToPlayer.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABossNyraClone::ABossNyraClone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABossNyraClone::BeginPlay()
{
	Super::BeginPlay();
}

void ABossNyraClone::Atack(APrimeiroGame* Mode)
{
	GameMode = Mode;
	PathPlayer = GetWorld()->SpawnActor<APathToPlayer>(PathPlayerClass, GetActorTransform());
	const int32 Direction = UKismetMathLibrary::RandomIntegerInRange(1, 3);
	FName Temp;
	if (Direction == 1)
		Temp = FName("CloneLeft");
	else if (Direction == 2)
		Temp = FName("CloneRight");
	else
		Temp = FName("CloneCenter");
	PathPlayer->SetPathPLayer(GetActorLocation(), GameMode->GetPlayer(), Direction);
	PlayAnimMontage(CloneMontage, 1.f, Temp);
	RunSpecialAtack();
}

void ABossNyraClone::RunSpecialAtackUpdate(const float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("tamanho: %f"), PathPlayer->GetPathLenght());
	const float Temp = FMath::Lerp(0.f, PathPlayer->GetPathLenght(), Value);
	SetActorLocationAndRotation(PathPlayer->GetLocationAlongSpline(Temp), PathPlayer->GetRotationAlongSpline(Temp));
	PathPlayer->UpdatePath(GameMode->GetPlayer());
}

void ABossNyraClone::RunSpecialAtackFinished()
{
	PathPlayer->Destroy();
}

void ABossNyraClone::KickAtack()
{
	PlayAnimMontage(CloneMontage, 1.f, "CloneKick");
}

void ABossNyraClone::DestroyClone()
{
	StopAnimMontage(CloneMontage);
	//pecisa desse timer para nao dar erro
	GetMesh()->SetVisibility(false);
	GetWorldTimerManager().SetTimer(TimerHandlerDestroye, this, &ABossNyraClone::RealyDestroy, 0.1f, false, 0.5f);
}

void ABossNyraClone::RealyDestroy()
{
	Destroy();
}


