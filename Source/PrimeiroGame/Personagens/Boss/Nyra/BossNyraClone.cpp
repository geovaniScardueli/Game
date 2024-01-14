// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNyraClone.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "PrimeiroGame/Personagens/Boss/PathToPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "PrimeiroGame/Personagens/Boss/AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

// Sets default values
ABossNyraClone::ABossNyraClone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleWeapon = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleWeapon->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ABossNyraClone::BeginPlay()
{
	Super::BeginPlay();
	//BossAIControlle = Cast<ABossAIController>(GetController());
	CapsuleWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
		FName("DEF-Sword1Socket"));

	bUseControllerRotationYaw = true;
	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		//deactivate this for not tilt/flic animation when move
		MovementComponent->bOrientRotationToMovement = false;
	}
	//BossAIControlle->GetBlackboardComponent()->SetValueAsBool("CanMov", false);
}

void ABossNyraClone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (StateEnemy == States::sAttack)
	{
		//const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// get forward vector
		//AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), 1);

		FRotator Rotacao = FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameMode->GetPlayer()->GetActorLocation()).Yaw, 0.f);
		RootComponent->SetWorldRotation(Rotacao);
		//UE_LOG(LogTemp, Warning, TEXT("tamanho: %f"), (float)Rotacao.Yaw);
		//GetController()->SetControlRotation(Rotacao);
	}
}

void ABossNyraClone::Atack(const FName CloneAttack)
{
	if (CloneAttack == "DisClone") {
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
	else if (CloneAttack == "ComboClone") {
		PlayAnimMontage(CloneMontages[CloneMontages::Attack], 1.f, "CloneAttackLeft");
	}
}

void ABossNyraClone::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("GRab:"));
	PlayAnimMontage(CloneMontages[CloneMontages::Grabb], 1.f, "Jump");
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
	if (Val == "Jump")
	{
		//GetWorldTimerManager().SetTimer(TimerHandlertarget, this, &ABossNyraClone::TargetPlayer, 0.1f, true, 0.1f);
		StateEnemy = States::sAttack;
		//SetActorLocation((GetActorLocation() + (GetActorForwardVector() * 70.f))*-1);
		PlayAnimMontage(CloneMontages[CloneMontages::Grabb], 1.f, Val);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	else
	{
		PlayAnimMontage(CloneMontages[CloneMontages::Attack], 1.f, Val);
	}
}

void ABossNyraClone::StartAttack(const FName IdAttack)
{
	if (IdAttack == "CloneGrab")
	{
		AtackSequence = 5;
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, false);
		CapsuleWeapon->DetachFromComponent(DetachmentRules);

		CapsuleWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			FName("HandL"));
		CapsuleWeapon->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 86.6f));
	}
	CapsuleWeapon->SetGenerateOverlapEvents(true);
}

void ABossNyraClone::ConectAttack(const FName Description)
{
	if (Description == "CloneGrab")
	{
		StopAttack();
		StateEnemy = States::sGrab;
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		SetActorLocation(GameMode->GetPlayer()->GetActorLocation());
		PlayAnimMontage(CloneMontages[CloneMontages::Grabb], 1.f, "Grab");
		//GetWorldTimerManager().SetTimer(TimerHandlertarget, this, &ABossNyraClone::TargetPlayer, 0.1f, false, 0.1f);
		//TargetPlayer();
	}
}

void ABossNyraClone::GrabEnd()
{
	OnCloneAction.Broadcast("GrabEnd");
}

void ABossNyraClone::GrabStart()
{
	SetActorLocation(GameMode->GetPlayer()->GetActorLocation());
}

void ABossNyraClone::StopAttack()
{
	CapsuleWeapon->SetGenerateOverlapEvents(false);
}


void ABossNyraClone::DestroyClone(const bool Fire)
{
	//StopAnimMontage(CloneMontage);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	if (Fire) OnCloneAction.Broadcast("Destroy");

	GetMesh()->SetVisibility(false);
	//pecisa desse timer para nao dar erro
	GetWorldTimerManager().SetTimer(TimerHandlerDestroye, this, &ABossNyraClone::RealyDestroy, 0.1f, false, 0.5f);
}

void ABossNyraClone::RealyDestroy()
{
	Destroy();
}

void ABossNyraClone::TargetPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("target aaaaaaa:"));
	FRotator Rotacao = FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameMode->GetPlayer()->GetActorLocation()).Yaw, 0.f);
	SetActorRotation(Rotacao);
}


