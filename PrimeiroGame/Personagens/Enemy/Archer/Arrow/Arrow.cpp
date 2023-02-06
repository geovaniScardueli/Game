// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//para a colisao funcionar a capsula deve ser o primeiro componente
	CapsuleArrow = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleArrow->OnComponentHit.AddDynamic(this, &AArrow::HitSomthing);
	RootComponent = CapsuleArrow;
	// Die after 10 seconds by default
	InitialLifeSpan = 10.0f;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();

	//CapsuleArrow->OnComponentEndOverlap.AddDynamic(this, &AArrow::OnoVerlapBla);

}

void AArrow::HitSomthing(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CapsuleArrow->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	if (AProtagonista* Player = Cast<AProtagonista>(OtherActor))
	{
		FVector OwnerDirection = OtherActor->GetActorForwardVector();
		FVector ActorDirection = GetActorLocation() - OtherActor->GetActorLocation();
		// Normalize vectors
		OwnerDirection.Normalize();
		ActorDirection.Normalize();

		// Calculate angle
		const float AnguloPlayer = FMath::RadiansToDegrees(acosf(FVector::DotProduct(OwnerDirection, ActorDirection)));
		if (Player->IsInDefensePosition())
		{
			if (AnguloPlayer < 75.f)
			{
				UE_LOG(LogTemp, Warning, TEXT("Osso: %s"), *Hit.BoneName.ToString());
				BounceArrow();
				return;
			}
		}
		Player->LevouPorrada(AnguloPlayer < 65.f);
	}
	Destroy();
}

void AArrow::OnoVerlapBla(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AProtagonista* Player = Cast<AProtagonista>(OtherActor))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Bone: %s"), *FString(SweepResult.BoneName.ToString));
		if (Player->IsInDefensePosition())
		{
			BounceArrow();
			return;
		}
		//Player->LevouPorrada();
	}
}

