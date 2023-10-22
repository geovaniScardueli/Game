// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNyraClone.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "PrimeiroGame/Personagens/Boss/PathToPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABossNyraClone::ABossNyraClone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleWeapon = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleWeapon->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ABossNyraClone::BeginPlay()
{
	Super::BeginPlay();

	CapsuleWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
		FName("DEF-Sword1Socket"));
}

void ABossNyraClone::Atack()
{
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
	PlayAnimMontage(CloneMontages[CloneMontages::Discharge], 1.f, Temp);
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

void ABossNyraClone::CloseAttack(const FName Val)
{
	PlayAnimMontage(CloneMontages[CloneMontages::Attack], 1.f, Val);
}

void ABossNyraClone::DestroyClone()
{
	//StopAnimMontage(CloneMontage);
	//pecisa desse timer para nao dar erro
	GetMesh()->SetVisibility(false);
	GetWorldTimerManager().SetTimer(TimerHandlerDestroye, this, &ABossNyraClone::RealyDestroy, 0.1f, false, 0.5f);
}

void ABossNyraClone::StartAttack()
{
	CapsuleWeapon->SetGenerateOverlapEvents(true);
}

void ABossNyraClone::StopAttack()
{
	CapsuleWeapon->SetGenerateOverlapEvents(false);
}

void ABossNyraClone::RealyDestroy()
{
	Destroy();
}


