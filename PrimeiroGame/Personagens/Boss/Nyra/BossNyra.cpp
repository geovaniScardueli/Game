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
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

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
	ChangeBlackboarValue("CanMov", true);
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
	if (!bCanMove) return;
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
	if (bUnstoppableAttack) return 2;

	//return 2;
	if (BlockAtack > 1000) return 0;

	RNumber = UKismetMathLibrary::RandomIntegerInRange(1, 100);

	if (RNumber < 30) return 4;
	if (RNumber < 80) return 1;

	return 2;
}

int32 ABossNyra::TakeHit(const int32 Power, const int32 Atack)
{	 
	///if (EAtackFury) return 2;
	EnableDisableOverBody(false);
	GetWorldTimerManager().SetTimer(TimerEnableOverlap, this, &ABossNyra::EnableOverlap, 0.1f, false, 0.4f);
	//return 0;
	switch (CheckWhatToDo())
	{
	case 0:
	{
		EnableDisableOverlapWeapon1(false);
		ChangeBlackboarValue("CanMov", false);
		PlayAnimMontage(Montages[AnimMontages::EDefense], 1, "Parry2");
		BlockAtack = 1;
		return 1;
	}
	case 1:
	{
		ChangeBlackboarValue("CanMov", false);
		EnableDisableOverlapWeapon1(false);
		PlayAnimMontage(Montages[AnimMontages::EDefense], 1, DefenseAnim[Atack]);
		BlockAtack *= RNumber;

		ChangeStamina(UDamage::CalcaletePosture(Power, Defense));
		return 2;
	}
	case 2:
	{
		BlockAtack *= RNumber;
		ChangeBlackboarValue("CanMov", false);
		LoseHealth(UDamage::CalcaleteHealth(Power, Defense));
		ChangeStamina(UDamage::CalcaletePosture(Power, Defense));
		if (!bUnstoppableAttack)
		{
			PlayAnimMontage(Montages[AnimMontages::EHit], 1, Hits[UKismetMathLibrary::FCeil(RNumber) / 33]);
		}
		return 0;
	}
	case 3:
	{
		ChangeBlackboarValue("CanMov", false);
		LoseHealth(UDamage::CalcaleteHealth(Power, Defense));
		ChangeStamina(UDamage::CalcaletePosture(Power, Defense));
		PlayAnimMontage(Montages[AnimMontages::EDash], 1, FName("Dash"));
		return 0;
	}
	case 4:
	{
		ChangeBlackboarValue("CanMov", false);
		PlayAnimMontage(Montages[AnimMontages::EAtack], 1.f, "Roll");
		//GetWorld()->SpawnActor<ABossNyraClone>(CloneClass, GetActorLocation() + FVector(0.f, 0.f, 1.f), GetActorRotation())->CloseAttack("Roll2");
		return 0;
	}
	default:
		return 0;
	}
}


void ABossNyra::EnableOverlap()
{
	EnableDisableOverBody(true);
}

void ABossNyra::EnableDisableOverlapWeapon1(const bool Enable)
{
	CapsuleWeapon->SetGenerateOverlapEvents(Enable);
}

void ABossNyra::EnableDisableOverlapWeapon2(const bool Enable)
{
	
}

void ABossNyra::ResetStatus(const FName Status)
{
	if (Status == "All")
	{
		UE_LOG(LogTemp, Warning, TEXT("entrou all"));
		EnableDisableOverlapWeapon1(false);
		AtackSequence = 0;
		bUnstoppableAttack = false;
		ChangeBlackboarValue("MoveToPLayer", false);
		ChangeBlackboarValue("AtackPlayer", false);
		ChangeBlackboarValue("CanMov", true);

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
	else if (Status == "Parry") {
		EnableDisableOverlapWeapon1(false);
	}
}

void ABossNyra::CheckAfterParry()
{

	if (RNumber > 90)
	{
		bUnstoppableAttack = true;
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
	AtackPlayer();
	ChangeBlackboarValue("MoveToPLayer", false);
	ChangeBlackboarValue("AtackPlayer", false);
	ChangeBlackboarValue("CanMov", false);
}

FName ABossNyra::GetMontageSection()
{
	return NyraAnimInstance->Montage_GetCurrentSection(Montages[EComboAtack]);
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
	UE_LOG(LogTemp, Warning, TEXT("chegou no attack"));
	if (bUnstoppableAttack)
	{
		PlayAnimMontage(Montages[AnimMontages::EComboAtack], 1, FName("Atack"));
		return;
	}
	const int32 SelectAttack = UKismetMathLibrary::RandomIntegerInRange(1, 100);
	FName NameAttack = FName("SingleAtack");
	if (SelectAttack > 85) {
		bUnstoppableAttack = true;
		Grab();
		return;
	}
	else if (SelectAttack > 70)
	{
		NameAttack = FName("AirAtack");
		bUnstoppableAttack = true;
	}
	PlayAnimMontage(Montages[AnimMontages::EAtack], 1, NameAttack);
	
	
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
	if (bUnstoppableAttack) return;

	ResetStatus("Parry");
	ChangeBlackboarValue("CanMov", false);
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
	Clone->Atack();
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
	Clone->CloseAttack("Kick");
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

void ABossNyra::StartAttack()
{
	ChangeBlackboarValue("CanMov", false);
	EnableDisableOverlapWeapon1(true);
	if (GetMontageSection().IsEqual("Atack", ENameCase::CaseSensitive) || GetMontageSection().IsEqual("Atack5", ENameCase::CaseSensitive))
	{
		ChangeCapsuleWeapon("Weapon");
	}
	else if (GetMontageSection().IsEqual("Atack3", ENameCase::CaseSensitive))
	{
		ChangeCapsuleWeapon("Perna");
	}
}

void ABossNyra::StopAttack()
{
	EnableDisableOverlapWeapon1(false);
}

void ABossNyra::ResetAllStatus()
{
	UE_LOG(LogTemp, Warning, TEXT("antes do alll entrou all"));
	ResetStatus("All");
}

void ABossNyra::StartTeleport()
{
	PlayAnimMontage(Montages[AnimMontages::ETeleport], 1.f, "StartTele");
}

void ABossNyra::CheckToTeleport()
{
	const FVector PlayerLocation = GameMode->GetPlayer()->GetActorLocation();

	//Location.Z -= 20;

	TArray<UPrimitiveComponent*> HitArray;
	TArray<AActor*> ActorToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsType;

	ObjectsType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<FHitResult> HitArrayDebug;

	const int16 Rnumber = UKismetMathLibrary::RandomIntegerInRange(1, 100);
	//0 = front, 1 = Left, 2 = Back, 3 = Right
	int16 Dir;
	if (Rnumber > 75) Dir = 0;
	else if (Rnumber > 50) Dir = 1;
	else if (Rnumber > 25) Dir = 2;
	else if (Rnumber > 0) Dir = 3;

	UE_LOG(LogTemp, Warning, TEXT("valor %d"), Dir);

	FVector TeleportLoca = PlayerLocation + GetLocationTeleport(Dir);

	bool Hit = UKismetSystemLibrary::CapsuleOverlapComponents(GetWorld(), TeleportLoca, 25.f, 88.f,
		ObjectsType, nullptr, ActorToIgnore, HitArray);

	if (Hit)
	{
		Dir++;
		if (Dir > 3) Dir = 0;
		TeleportLoca = PlayerLocation + GetLocationTeleport(Dir);
		Hit = UKismetSystemLibrary::CapsuleOverlapComponents(GetWorld(), TeleportLoca, 25.f, 88.f,
			ObjectsType, nullptr, ActorToIgnore, HitArray);
		if (Hit)
		{
			Dir++;
			if (Dir > 3) Dir = 0;
			TeleportLoca = PlayerLocation + GetLocationTeleport(Dir);
			Hit = UKismetSystemLibrary::CapsuleOverlapComponents(GetWorld(), TeleportLoca, 25.f, 88.f,
				ObjectsType, nullptr, ActorToIgnore, HitArray);
			if (Hit)
			{
				Dir++;
				if (Dir > 3) Dir = 0;
				TeleportLoca = PlayerLocation + GetLocationTeleport(Dir);
				Hit = UKismetSystemLibrary::CapsuleOverlapComponents(GetWorld(), TeleportLoca, 25.f, 88.f,
					ObjectsType, nullptr, ActorToIgnore, HitArray);
			}
		}
	}

	SetActorLocation(TeleportLoca);
	PlayAnimMontage(Montages[AnimMontages::ETeleport], 1.f, "TeleArrive");
}

FVector ABossNyra::GetLocationTeleport(const int16 Val)
{
	switch (Val)
	{
	case 0:
		return GameMode->GetPlayer()->GetActorForwardVector() * 70.f;
	case 1:
		return GameMode->GetPlayer()->GetActorRightVector() * 70.f * -1;
	case 2:
		return GameMode->GetPlayer()->GetActorForwardVector() * 70.f * -1;
	case 3:
		return GameMode->GetPlayer()->GetActorRightVector() * 70.f;
	default:
		return GameMode->GetPlayer()->GetActorForwardVector() * 70.f;
	}
}
