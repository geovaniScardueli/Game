// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaulEnemyHead.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADefaulEnemyHead::ADefaulEnemyHead()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = DefaultSceneRoot;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void ADefaulEnemyHead::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADefaulEnemyHead::FireHead()
{
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->AddImpulse(FVector(UKismetMathLibrary::RandomIntegerInRange(XVelocity, 800),
		UKismetMathLibrary::RandomIntegerInRange(-1500, YVelocity),
		UKismetMathLibrary::RandomIntegerInRange(ZVelocity, 800)));
}
