// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNyra.h"
#include "PrimeiroGame/Personagens/Boss/PathToPlayer.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/UI/InimigoPadraoWidget.h"
#include "PrimeiroGame/Personagens/Boss/AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossNyra::ABossNyra()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CapsuleWeapon = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleWeapon->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ABossNyra::BeginPlay()
{
	Super::BeginPlay();
	BossAIControlle = Cast<ABossAIController>(GetController());
	bSeePlayer = false;
	ChangeBlackboarValue("CanMove", true);
	ChangeVisibilityUI(true);
}

// Called every frame
void ABossNyra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotacao = FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameMode->GetPlayer()->GetActorLocation()).Yaw, 0.f);
	Rotacao.Pitch = 0.f;
	Rotacao.Roll = 0.f;
	SetActorRotation(Rotacao);
	GetController()->SetControlRotation(Rotacao);
}

void ABossNyra::BossSpecialAtack()
{
	PathPlayer = GetWorld()->SpawnActor<APathToPlayer>(PathPlayerClass, GetActorTransform());
	PathPlayer->SetPathPLayer(GetActorLocation(), GameMode->GetPlayer());
	RunSpecialAtack();
	//PathPlayer->PathLenght();
}

void ABossNyra::TesteMethod()
{
	//BossSpecialAtack();
	PlayAnimMontage(ArrayMontage[0], 1, FName("Atack"));
	
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

void ABossNyra::BossTakeHit(const int16 DamageTake, const float StaminaDamage, int16 animation)
{

	BlockAtack *= 8;
	ChangeBlackboarValue("CanMove", false);
	EnableDisableOverBody(false);
	VidaAtual -= DamageTake;
	Stamina += StaminaDamage;
	if (VidaAtual < 0) VidaAtual = 0;
	if (Stamina > 100.f)  Stamina = 100.f;
	PlayAnimMontage(Montages[BossMontages::BHit], 1, Hits[animation]);
}

void ABossNyra::EnableDisableOverlapWeapon1(const bool Enable)
{
	CapsuleWeapon->SetGenerateOverlapEvents(Enable);
}

void ABossNyra::EnableDisableOverlapWeapon2(const bool Enable)
{
	
}

void ABossNyra::ResetAllStatus()
{
	EnableDisableOverBody(true);
	ChangeBlackboarValue("MoveToPLayer", false);
	ChangeBlackboarValue("AtackPlayer", false);
	ChangeBlackboarValue("CanMove", true);
}

void ABossNyra::AtackPlayer()
{
	if (!bCanMove) return;

	PlayAnimMontage(Montages[BossMontages::BAtack], 1, FName("Atack"));
}

void ABossNyra::ChangeBlackboarValue(const FName Description, bool Val)
{
	if (Description.ToString().Equals("CanMov"))
	{
		bCanMove = Val;
	}
	BossAIControlle->GetBlackboardComponent()->SetValueAsBool(Description, Val);
}

void ABossNyra::ParryAnimation(float Val, FVector LocationPlayer)
{
	ChangeBlackboarValue("CanMove", false);
	PlayAnimMontage(Montages[BossMontages::BParry], 1, FName("TakeParry1"));
}

void ABossNyra::EnableDisableOverBody(const bool Enable)
{
	if (Enable)
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	else
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
}
