// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathToPlayer.generated.h"

class AProtagonista;

UCLASS()
class PRIMEIROGAME_API APathToPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathToPlayer();

	UFUNCTION()
	void SetPathPLayer(const FVector StartLocation, const AProtagonista* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, category="fonte propriedades")
	class USplineComponent* Path;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	float GetPathLenght();

	UFUNCTION()
	FVector GetLocationAlongSpline(const float Value);

	UFUNCTION()
	FRotator GetRotationAlongSpline(const float Value);


};
