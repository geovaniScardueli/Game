// Fill out your copyright notice in the Description page of Project Settings.


#include "Inimigo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "PrimeiroGame/Personagens/EnemyAnimInstance.h"
#include "PrimeiroGame/Personagens/AI/EnemyAIController.h"
#include "InimigoPadrao/UI/InimigoPadraoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "Protagonista/Protagonista.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InimigoPadrao/Head/DefaulEnemyHead.h"

// Sets default values
AInimigo::AInimigo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyWidgetComp = CreateDefaultSubobject<UWidgetComponent>("EnemyWidget");	
	EnemyWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	EnemyWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	EnemyWidgetComp->SetDrawAtDesiredSize(true);
	
	//static ConstructorHelpers::FClassFinder<UUserWidget> PlayerPawnBPClass(TEXT("/Game/Personagens/UI/Enemy/WG_EnemyBP.WG_EnemyBP"));
	//if (PlayerPawnBPClass.Succeeded())
	//{
	//	EnemyWidgetComp->SetWidgetClass(PlayerPawnBPClass.Class);
	//}
}

// Called when the game starts or when spawned
void AInimigo::BeginPlay()
{
	Super::BeginPlay();
	EnemyAIControlle = Cast<AEnemyAIController>(GetController());
	//ChangeBlackboarValue("CanMov", true);
	GameMode = Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode());
	if (EnemyDefaultWidgetClass)
	{
		EnemyWidgetComp->SetWidgetClass(EnemyDefaultWidgetClass);
	}
	ChangeVisibilityUI(false);

	//EnemyDefaultWidget->SetVisibility()
	//EnemyWidgetComp->SetWidgetClass(EnemyDefaultWidget);
}

void AInimigo::ComecarMoverUpdate(float Value)
{
	FVector Location = GetActorLocation();
	Location.Y = FMath::Lerp(700.f, 1000.f, Value);
	SetActorLocation(Location);
}

void AInimigo::ComecarMoverFinished()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Finilizar Mover")));
}

// Called every frame
void AInimigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bSeePlayer)
	{
		FRotator Rotacao = FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameMode->GetPlayer()->GetActorLocation()).Yaw, 0.f);
		Rotacao.Pitch = 0.f;
		Rotacao.Roll = 0.f;
		SetActorRotation(Rotacao);
		GetController()->SetControlRotation(Rotacao);
	}
}

int AInimigo::TakeHit(AActor* OtherActor, int Damage, float DamageStamina, int animation)
{
	//FString Comp = OtherComp->GetName();
	//if (Comp.Equals(TEXT("CapsuleWeapon")))
	//{
	//GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Vehicle, ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	ChangeBlackboarValue("CanMov", false);
	//ChangeBlackboarValue("ViuPlayer", false);

	if (bSeePlayer &&  BlockAtack > UKismetMathLibrary::RandomInteger(100))
	{
		BlockAtack = 10;
		PlayAnimMontage(Montages[EEnemyMontages::EEHit], 1, FName("ParryPlayer"));
		ChangeBlackboarValue("CanMov", true);
		ChangeBlackboarValue("AtacarPlayer", true);
		ChangeBlackboarValue("IsRangeAtack", true);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
		return 1;
	}
	BlockAtack*=2;
	LoseHealth(Damage);
	if (VidaAtual <= 0)
	{
		EnemyAIControlle->DisableBehaviorTree();
		PlayAnimMontage(Montages[EEnemyMontages::EEDie], 1, FName("Die"));
		DisableInDead();
		return 0;
	}
	ChangeStamina(DamageStamina);
	if (Stamina == 100.f)
	{
		bSeePlayer = false;
		PlayAnimMontage(Montages[EEnemyMontages::EEHit], 1, FName("StaminaOver"));
		if (Cast<AProtagonista>(OtherActor)->ValidarEnemigoCampoVisao(this)) ChangeExecuteMode(1.f);
	} else
	{
		if (animation == 0)
		{
			PlayAnimMontage(Montages[EEnemyMontages::EEHit], 1, FName("TakeFistHit"));
		}
		else
		{
			PlayAnimMontage(Montages[EEnemyMontages::EEHit], 1, FName("TakeSecondHit"));
		}
	}
	return 0;
}

void AInimigo::TakeExecutionPerfectParry(FVector FowardPlayer)
{
	bSeePlayer = false;
	EnemyAIControlle->DisableBehaviorTree();
	//SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 150 *-1));
	PlayAnimMontage(Montages[EEnemyMontages::EEExecution], 1, FName("TakeExecutionParry"));
	LoseHealth(999);
	DisableInDead();
}

void AInimigo::Decapitated()
{
	GetMesh()->HideBoneByName(FName("DEF-spine_006"), EPhysBodyOp::PBO_None);
	FTransform Temp = GetMesh()->GetSocketTransform(FName("DEF-spine_006"));
	AActor* Cabeca = GetWorld()->SpawnActor<ADefaulEnemyHead>(HeadClass, Temp);
	if (Cabeca) {
		Cast<ADefaulEnemyHead>(Cabeca)->FireHead();
	}
}

void AInimigo::TesteTemp()
{
	GetMesh()->HideBoneByName(FName("DEF-spine_006"), EPhysBodyOp::PBO_None);
	FTransform Temp = GetMesh()->GetSocketTransform(FName("DEF-spine_006"));
	AActor* Cabeca = GetWorld()->SpawnActor<ADefaulEnemyHead>(HeadClass, Temp);
	if (Cabeca) {
		Cast<ADefaulEnemyHead>(Cabeca)->FireHead();
	}
}

void AInimigo::LoseHealth(int Val)
{
	VidaAtual -= Val;
	if (VidaAtual < 0) VidaAtual = 0;
	//Cast<UInimigoPadraoWidget>(EnemyWidgetComp->GetWidgetClass())->AtualizarVida(VidaAtual);
	//EnemyDefaultWidget->AtualizarVida(VidaAtual);
	Cast<UInimigoPadraoWidget>(EnemyWidgetComp->GetUserWidgetObject())->AtualizarVida(VidaAtual);
}

void AInimigo::ComecarAMover()
{
	ComecarMover();
}

void AInimigo::ParryAnimation(float Val, FVector LocationPlayer)
{
	ChangeBlackboarValue("CanMov", false);
	EnableDisabelOverlapWeapon(false);
	ChangeStamina(Val);
	if (Stamina == 100.f)
	{
		FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LocationPlayer);
		Rotacao.Pitch = 0.f;
		Rotacao.Roll = 0.f;
		SetActorRotation(Rotacao);
		//SetActorLocation(LocationPlayer * 200 * -1);
		//LaunchCharacter(FVector(0, 620.f, 170.f), false, false);
		PlayAnimMontage(Montages[EEnemyMontages::EEExecution], 1, FName("TakeExecutinAwait"));
	} 
	else
	{
		PlayAnimMontage(Montages[EEnemyMontages::EEAtack], 1, FName("Parry"));
	}
}

void AInimigo::OnTakeExecution(bool bIsInFront, AActor* Player2)
{
	ChangeBlackboarValue("CanMov", false);
	if (bIsInFront)
	{
		SetActorRotation(FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player2->GetActorLocation()).Yaw, 0.f));
	} else
	{
		SetActorRotation(Player2->GetActorRotation());
	}
	PlayAnimMontage(Montages[EEnemyMontages::EEExecution], 1, bIsInFront ? FName("TakeExecutionFront") : FName("TakeExecution"));
	DisableInDead();
}

void AInimigo::ChangeStamina(float Val)
{
	Stamina += Val;
	if (Stamina >= 100.f) Stamina = 100.f;
	if (Stamina < 0.f) Stamina = 0.f;
	ChangeVisibilityUI(true);
	//EnemyDefaultWidget->AtualizaStamina(Stamina);
}

//jogar para cada classe especifica, 217 nullpointer


bool AInimigo::GetBlackboarValue(const FName Description)
{
	return EnemyAIControlle->GetBlackboardComponent()->GetValueAsBool(Description);
}

bool AInimigo::IsDead()
{
	return Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->IsDead();
}

void AInimigo::DisableInDead()
{
	ChangeVisibilityUI(false);
	ChangeExecuteMode(0.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
	EnemyAIControlle->DisableBehaviorTree();
}

void AInimigo::SpecialAtack(FVector Location)
{
	ChangeBlackboarValue("SpecialAtack", true);
	SetActorLocation(Location);
	FRotator Rotacao = FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameMode->GetPlayer()->GetActorLocation()).Yaw, 0.f);
	Rotacao.Pitch = 0.f;
	Rotacao.Roll = 0.f;
	SetActorRotation(Rotacao);
	GetController()->SetControlRotation(Rotacao);
	PlayAnimMontage(Montages[EEnemyMontages::EESpecialAtack], 1, FName("SpecialAtack"));
}

void AInimigo::ChangeVisibilityUI(const bool Val)
{
	EnemyWidgetComp->SetVisibility(Val);
}
