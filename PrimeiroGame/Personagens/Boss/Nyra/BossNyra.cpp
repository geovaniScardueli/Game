// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNyra.h"
#include "PrimeiroGame/Personagens/Boss/PathToPlayer.h"
#include "PrimeiroGame/PrimeiroGame.h"

// Sets default values
ABossNyra::ABossNyra()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABossNyra::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ABossNyra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossNyra::SpecialAtack()
{
	PathPlayer = GetWorld()->SpawnActor<APathToPlayer>(PathPlayerClass, GetActorTransform());
	PathPlayer->SetPathPLayer(GetActorLocation(), GameMode->GetPlayer());
	RunSpecialAtack();
	//PathPlayer->PathLenght();
}

void ABossNyra::TesteMethod()
{
	SpecialAtack();
}

void ABossNyra::RunSpecialAtackUpdate(const float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("tamanho: %f"), PathPlayer->GetPathLenght());
	const float Temp = FMath::Lerp(0.f, PathPlayer->GetPathLenght(), Value);
	SetActorLocationAndRotation(PathPlayer->GetLocationAlongSpline(Temp), PathPlayer->GetRotationAlongSpline(Temp));
}

void ABossNyra::RunSpecialAtackFinished()
{

}