// Fill out your copyright notice in the Description page of Project Settings.


#include "PathToPlayer.h"
#include "Components/SplineComponent.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

// Sets default values
APathToPlayer::APathToPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Caminho"));
}

// Called when the game starts or when spawned
void APathToPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APathToPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APathToPlayer::GetPathLenght()
{
	return Path->GetSplineLength() - 70.f;
}

FVector APathToPlayer::GetLocationAlongSpline(const float Value)
{
	return Path->GetLocationAtDistanceAlongSpline(Value, ESplineCoordinateSpace::World);
}

FRotator APathToPlayer::GetRotationAlongSpline(const float Value)
{
	return Path->GetRotationAtDistanceAlongSpline(Value, ESplineCoordinateSpace::World);
}

void APathToPlayer::SetPathPLayer(const FVector StartLocation, const AProtagonista* Player)
{
	const FVector PlayerLocation = Player->GetActorLocation();
	Path->SetLocationAtSplinePoint(0, StartLocation, ESplineCoordinateSpace::World, false);
	Path->SetLocationAtSplinePoint(2, PlayerLocation, ESplineCoordinateSpace::World, true);
	const FVector Half = (StartLocation + PlayerLocation) / 2 + FVector(200.f, 0.f, 0.f);
	Path->SetLocationAtSplinePoint(1, Half, ESplineCoordinateSpace::World, true);
}
