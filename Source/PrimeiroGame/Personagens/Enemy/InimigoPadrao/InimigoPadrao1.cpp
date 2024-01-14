// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoPadrao1.h"

#include "AI/InimigoAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/AI/InimigoAIController.h"

// Sets default values
AInimigoPadrao1::AInimigoPadrao1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CapsuleWeapon = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleWeapon->SetupAttachment(GetMesh());
	
}

// Called when the game starts or when spawned
void AInimigoPadrao1::BeginPlay()
{
	Super::BeginPlay();
	InimigoPadraoAIControlle = Cast<AInimigoAIController>(GetController());
	GetMesh()->HideBoneByName(FName("DEF-Bone"), EPhysBodyOp::PBO_None);
	SwordBP = GetWorld()->SpawnActor<AActor>(SwordClass, GetMesh()->GetSocketTransform(FName("DEF-Bone")));

	SwordBP->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true),
		FName("WeaponSocket"));

}

// Called every frame
void AInimigoPadrao1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInimigoPadrao1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInimigoPadrao1::TesteAiController()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("entrou no ai controleer no inimigo"));
	if (bAndou)
	{
		Cast<AInimigoAIController>(GetController())->MoveToLocation(FVector(740.f, -490.f,598.f));
	} else
	{
		Cast<AInimigoAIController>(GetController())->MoveToLocation(FVector(740.f, 320.f,598.f));
	}
	bAndou = !bAndou;
}

void AInimigoPadrao1::AtackPlayer()
{
	if (bCanMove)
	{
		if (FVector::Dist(GetActorLocation(), GetPlayer()->GetActorLocation()) < 100.f)
		{
			PlayAnimMontage(Montages[AnimMontages::EAtack], 1, FName("CloseAtack"));
		}
		else 
		{
			PlayAnimMontage(Montages[AnimMontages::EAtack], 1, FName("AtackPlayer"));
		}
	}
}

void AInimigoPadrao1::ChangeBlackboarValue(const FName Description, bool Val)
{
	if (Description.ToString().Equals("CanMov"))
	{
		bCanMove = Val;
	}
	InimigoPadraoAIControlle->GetBlackboardComponent()->SetValueAsBool(Description, Val);
}

void AInimigoPadrao1::SensePlayer()
{
	SetSeePlayer(true);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	ChangeBlackboarValue(TEXT("ViuPlayer"), true);
}

void AInimigoPadrao1::PhysicsWeapon()
{
	EnablePhysicsWeapon();
}

