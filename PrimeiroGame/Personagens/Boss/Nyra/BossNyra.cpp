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
#include "PrimeiroGame/Personagens/Boss/Nyra/animInstance/NyraAnimInstance.h"
#include "PrimeiroGame/Personagens/Damage/Damage.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyraClone.h"

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
	NyraAnimInstance = Cast<UNyraAnimInstance>(GetMesh()->GetAnimInstance());
	bSeePlayer = false;
	ChangeBlackboarValue("CanMove", true);
	ChangeVisibilityUI(true);

	CapsuleWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
		FName("DEF-Sword1Socket"));

	//CapsuleWeapon->AttachToComponent(GetMesh(),
	//	FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
	//	FName("PernaR"));
	
	//CapsuleWeapon->SetRelativeTransform();
	ChangeCapsuleWeapon("Weapon");
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
	//PathPlayer = GetWorld()->SpawnActor<APathToPlayer>(PathPlayerClass, GetActorTransform());
	//PathPlayer->SetPathPLayer(GetActorLocation(), GameMode->GetPlayer());
	//RunSpecialAtack();
	//PathPlayer->PathLenght();
}

void ABossNyra::RunSpecialAtackUpdate(const float Value)
{
	const float Temp = FMath::Lerp(0.f, PathPlayer->GetPathLenght(), Value);
	SetActorLocationAndRotation(PathPlayer->GetLocationAlongSpline(Temp), PathPlayer->GetRotationAlongSpline(Temp));
}

void ABossNyra::RunSpecialAtackFinished()
{

}


int32 ABossNyra::CheckWhatToDo()
{

	if (BlockAtack > 1000) return 0;

	RNumber = UKismetMathLibrary::RandomIntegerInRange(1, 100);

	if (RNumber < 80) return 1;

	return 2;
}

int32 ABossNyra::TakeHit(const int32 Power)
{	 
	///if (EAtackFury) return 2;
	EnableDisableOverBody(false);
	
	//return 0;
	switch (CheckWhatToDo())
	{
	case 0:
	{
		ChangeBlackboarValue("CanMove", false);
		EnableDisableOverlapWeapon1(false);
		PlayAnimMontage(Montages[AnimMontages::EExecParry], 1, "Parry2");
		BlockAtack = 1;
		return 1;
	}
	case 1:
	{
		
		ChangeBlackboarValue("CanMove", false);
		EnableDisableOverlapWeapon1(false);
		PlayAnimMontage(Montages[AnimMontages::EExecParry], 1, "Parry1");
		BlockAtack *= RNumber;

		ChangeStamina(UDamage::CalcaletePosture(Power, Defense));
		return 2;
	}
	case 2:
	{
		BlockAtack *= RNumber;
		ChangeBlackboarValue("CanMove", false);
		LoseHealth(UDamage::CalcaleteHealth(Power, Defense));
		ChangeStamina(UDamage::CalcaletePosture(Power, Defense));
		PlayAnimMontage(Montages[AnimMontages::EHit], 1, Hits[UKismetMathLibrary::FCeil(RNumber)/33]);
		return 0;
	}
	case 3:
	{
		ChangeBlackboarValue("CanMove", false);
		LoseHealth(UDamage::CalcaleteHealth(Power, Defense));
		ChangeStamina(UDamage::CalcaletePosture(Power, Defense));
		PlayAnimMontage(Montages[AnimMontages::EDash], 1, FName("Dash"));
		return 0;
	}
	default:
		return 0;
	}
	/*
	if (EAtackFury) return 2;
	RNumber = UKismetMathLibrary::RandomIntegerInRange(1, 10);
	if (animation > 2) animation = 1;
	if (BlockAtack > 100)
	{
		ChangeBlackboarValue("CanMove", false);
		EnableDisableOverlapWeapon1(false);
		PlayAnimMontage(Montages[AnimMontages::BExecParry], 1, "Parry2");
		BlockAtack = 1;
		return 1;
	}
	if (RNumber <8)
	{
		ChangeBlackboarValue("CanMove", false);
		EnableDisableOverlapWeapon1(false);
		PlayAnimMontage(Montages[AnimMontages::BExecParry], 1, "Parry1");
		BlockAtack *= RNumber;
		Stamina += StaminaDamage/2;
		Cast<UInimigoPadraoWidget>(EnemyWidgetComp->GetUserWidgetObject())->AtualizaStamina(Stamina);
		return 2;
	}
	BlockAtack *= RNumber;
	ChangeBlackboarValue("CanMove", false);
	EnableDisableOverBody(false);
	VidaAtual -= DamageTake;
	Stamina += StaminaDamage;
	if (VidaAtual < 0) VidaAtual = 0;
	if (Stamina > 100.f)  Stamina = 100.f;
	PlayAnimMontage(Montages[AnimMontages::EHit], 1, Hits[animation]);
	Cast<UInimigoPadraoWidget>(EnemyWidgetComp->GetUserWidgetObject())->AtualizarVida(VidaAtual);
	Cast<UInimigoPadraoWidget>(EnemyWidgetComp->GetUserWidgetObject())->AtualizaStamina(Stamina);
	return 0;
	*/
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
	AtackSequence = 0;
	bAtackFury = false;
	EnableDisableOverBody(true);
	ChangeBlackboarValue("MoveToPLayer", false);
	ChangeBlackboarValue("AtackPlayer", false);
	ChangeBlackboarValue("CanMove", true);

	if (CapsuleWeapon->GetAttachSocketName() != "DEF-Sword1Socket") {
		ChangeCapsuleWeapon("Weapon");
	}

	if (LeftEye)
	{
		LeftEye->Destroy();
	}
	if (RightEye)
	{
		RightEye->Destroy();
	}
}

void ABossNyra::CheckAfterParry()
{
	EnableDisableOverBody(true);
	ChangeBlackboarValue("MoveToPLayer", false);
	ChangeBlackboarValue("AtackPlayer", true);
	ChangeBlackboarValue("CanMove", true);
	if (RNumber > 500)
	{
		bAtackFury = true;
		//FTransform Temp = GetMesh()->GetSocketTransform(FName("RightEye"));
		//AActor* Cabeca = GetWorld()->SpawnActor<ADefaulEnemyHead>(HeadClass, Temp);
		RightEye = GetWorld()->SpawnActor<AActor>(Eye, GetMesh()->GetSocketTransform(FName("RightEye")));
		RightEye->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			FName("RightEye"));

		LeftEye = GetWorld()->SpawnActor<AActor>(Eye, GetMesh()->GetSocketTransform(FName("LeftEye")));
		LeftEye->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			FName("LeftEye"));
	}
}

FName ABossNyra::GetMontageSection()
{
	return NyraAnimInstance->Montage_GetCurrentSection(Montages[1]);
}

void ABossNyra::ChangeCapsuleWeapon(const FName Socket)
{
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, false);
	CapsuleWeapon->DetachFromComponent(DetachmentRules);
	if ("Weapon" == Socket)
	{
		CapsuleWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			FName("DEF-Sword1Socket"));
		CapsuleWeapon->SetRelativeLocationAndRotation(FVector(0.f, 33.f, -0.1f), FRotator(0.f, 0.f, -89.7f));
	}
	else if ("Perna" == Socket)
	{
		CapsuleWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			FName("PernaR"));
		CapsuleWeapon->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 86.6f));
	}
	else if ("Hand" == Socket)
	{
		CapsuleWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
			FName("HandL"));
		CapsuleWeapon->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 86.6f));
	}
}

void ABossNyra::AtackPlayer()
{
	if (!bCanMove) return;
	PlayAnimMontage(Montages[AnimMontages::EAtack], 1, FName("Atack"));
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
	//return;
	if (bAtackFury) return;

	ChangeBlackboarValue("CanMove", false);
	PlayAnimMontage(Montages[AnimMontages::EParry], 1, FName("TakeParry1"));
}

void ABossNyra::EnableDisableOverBody(const bool Enable)
{
	if (Enable) {
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	}
	else {
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	}
}

void ABossNyra::DischargeSingleClone()
{
	CloneAtack = 4;
	PlayAnimMontage(Montages[AnimMontages::ECreateClone], 1, FName("SoloDischarge"));
}

void ABossNyra::CreateClone()
{
	float Distance = FVector::Dist(GetActorLocation(), GameMode->GetPlayer()->GetActorLocation());
	Clone = GetWorld()->SpawnActor<ABossNyraClone>(CloneClass, GetActorLocation() + FVector(0.f, 0.f, 1.f), GetActorRotation());
	Clone->Atack(GameMode);
}

void ABossNyra::Grab()
{
	AtackSequence = 5;
	ChangeCapsuleWeapon("Hand");
	PlayAnimMontage(Montages[AnimMontages::EGrab], 1, FName("Grab"));
}

void ABossNyra::CreateCloneKick()
{
	Clone = GetWorld()->SpawnActor<ABossNyraClone>(CloneClass, GetActorLocation() + FVector(0.f, 0.f, 1.f), GetActorRotation());
	Clone->KickAtack();
}

void ABossNyra::ThrowGrab()
{
	PlayAnimMontage(Montages[AnimMontages::EGrab], 1, FName("ThrowGrab"));
}

void ABossNyra::TesteMethod()
{
	//DischargeSingleClone();
	Grab();
	//PlayAnimMontage(Montages[AnimMontages::ECreateClone], 1, FName("Kick"));
}

