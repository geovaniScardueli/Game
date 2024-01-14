// Fill out your copyright notice in the Description page of Project Settings.


#include "PontoMoventacao.h"

#include "Components/SphereComponent.h"
#include "PrimeiroGame/PrimeiroGame.h"

// Sets default values
APontoMoventacao::APontoMoventacao()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EsferaColisao = CreateDefaultSubobject<USphereComponent>(TEXT("Colisao"));
	RootComponent = EsferaColisao;

	Malha = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malha"));
	Malha->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APontoMoventacao::BeginPlay()
{
	Super::BeginPlay();
	EsferaColisao->OnComponentBeginOverlap.AddDynamic(this, &APontoMoventacao::OnEnterRaio);
	EsferaColisao->OnComponentEndOverlap.AddDynamic(this, &APontoMoventacao::OnSairRaio);
	
}

// Called every frame
void APontoMoventacao::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APontoMoventacao::OnEnterRaio(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->ActorHasTag("Player"))
	{
		Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode())->SetLocalMovimentacao(this->GetActorLocation());
		MeshValue = 1.f;
		OnChangeMesh();
	}
}

void APontoMoventacao::OnSairRaio(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode())->ResetLocalMovimentacao();
		MeshValue = 0.f;
		OnChangeMesh();
	}
}

