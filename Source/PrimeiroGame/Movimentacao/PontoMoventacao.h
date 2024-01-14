// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PontoMoventacao.generated.h"

UCLASS()
class PRIMEIROGAME_API APontoMoventacao : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APontoMoventacao();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Esfera")
	class USphereComponent* EsferaColisao;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Esfera")
	class UStaticMeshComponent* Malha;

	UPROPERTY(BlueprintReadOnly, Category="Movimentacao")
	float MeshValue = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Movimentacao")
	void OnChangeMesh();

private:
	UFUNCTION()
	void OnEnterRaio(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSairRaio(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
