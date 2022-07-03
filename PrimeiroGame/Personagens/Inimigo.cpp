// Fill out your copyright notice in the Description page of Project Settings.


#include "Inimigo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "InimigoPadrao/InimigoPadraoAnimInstance.h"
#include "InimigoPadrao/AI/InimigoPadraoAIController.h"
#include "InimigoPadrao/UI/InimigoPadraoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "Protagonista/Protagonista.h"

// Sets default values
AInimigo::AInimigo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetDrawAtDesiredSize(true);
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerPawnBPClass(TEXT("/Game/Personagens/InimigoPadrao/UI/BP_HealthBarInimigo.BP_HealthBarInimigo_C"));
	if (PlayerPawnBPClass.Succeeded())
	{
		HealthBar->SetWidgetClass(PlayerPawnBPClass.Class);
	}

	CapsuleWeapon = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleWeapon->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AInimigo::BeginPlay()
{
	Super::BeginPlay();
	ChangeBlackboarValue("CanMov", true);
	GameMode = Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode());
	//GetCharacterMovement()->bori
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

void AInimigo::TakeHit(AActor* OtherActor, int Damage, float DamageStamina, int animation)
{
	//FString Comp = OtherComp->GetName();
	//if (Comp.Equals(TEXT("CapsuleWeapon")))
	//{
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	Cast<AInimigoPadraoAIController>(GetController())->StopMovement();
	ChangeBlackboarValue("CanMov", false);
	ChangeStamina(DamageStamina);
	int Temp = UKismetMathLibrary::RandomInteger(100);
	UE_LOG(LogTemp, Warning, TEXT("Random: %d Atual: %d"), Temp, BlockAtack);
	if (BlockAtack > UKismetMathLibrary::RandomInteger(100))
	{
		BlockAtack = 10;
		PlayAnimMontage(TakeHitAnimMontage, 1, FName("ParryPlayer"));
		return;
	}
	BlockAtack*=2;
	TakeDamage(Damage);
	if (VidaAtual <= 0)
	{
		bIsDead = true;
		Cast<AInimigoPadraoAIController>(GetController())->DisableBehaviorTree();
		Cast<UInimigoPadraoAnimInstance>(GetMesh()->GetAnimInstance())->CharacterIsDead();
		PlayAnimMontage(Die, 1, FName("Die"));
	} else if (Stamina == 100.f)
	{
		PlayAnimMontage(TakeHitAnimMontage, 1, FName("StaminaOver"));
		if (Cast<AProtagonista>(OtherActor)->ValidarEnemigoCampoVisao(this)) ChangeExecuteMode(1.f);
	} else
	{
		if (animation == 0)
		{
			PlayAnimMontage(TakeHitAnimMontage, 1, FName("TakeFistHit"));
		}
		else
		{
			PlayAnimMontage(TakeHitAnimMontage, 1, FName("TakeSecondHit"));
		}
		
	}
}

void AInimigo::TakeDamage(int Val)
{
	LaunchCharacter(-GetActorForwardVector() * 50.f, true, false);
	VidaAtual -= Val;
	if (VidaAtual < 0) VidaAtual = 0;
	UInimigoPadraoWidget* Widget = Cast<UInimigoPadraoWidget>(HealthBar->GetUserWidgetObject());
	Widget->AtualizarVida(VidaAtual);
}

void AInimigo::ComecarAMover()
{
	ComecarMover();
}

void AInimigo::AtackPlayer()
{
	if (bCanMove)
	{
		PlayAnimMontage(Atack, 1, FName("AtackPlayer"));
	}
}

void AInimigo::ParryAnimation(float Val)
{
	ChangeBlackboarValue("CanMov", false);
	EnableDisabelOverlapWeapon(false);
	ChangeStamina(Val);
	if (Stamina == 100.f)
	{
		PlayAnimMontage(TakeExecution, 1, FName("StaminaOver"));
	} else
	{
		PlayAnimMontage(Atack, 1, FName("Parry"));
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
	PlayAnimMontage(TakeExecution, 1, FName("TakeExecution"));
	Cast<UInimigoPadraoAnimInstance>(GetMesh()->GetAnimInstance())->CharacterIsDead();
	ChangeExecuteMode(0.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Cast<AInimigoPadraoAIController>(GetController())->DisableBehaviorTree();
}

void AInimigo::ChangeStamina(float Val)
{
	Stamina += Val;
	if (Stamina >= 100.f) Stamina = 100.f;
	if (Stamina < 0.f) Stamina = 0.f;
	ChangeVisibilityUI(true);
	UInimigoPadraoWidget* Widget = Cast<UInimigoPadraoWidget>(HealthBar->GetUserWidgetObject());
	Widget->AtualizaStamina(Stamina);
}

void AInimigo::ChangeBlackboarValue(const FName Description, bool Val)
{
	if (Description.ToString().Equals("CanMov"))
	{
		bCanMove = Val;
	}
	
	Cast<AInimigoPadraoAIController>(GetController())->GetBlackboardComponent()->SetValueAsBool(Description, Val);
}

