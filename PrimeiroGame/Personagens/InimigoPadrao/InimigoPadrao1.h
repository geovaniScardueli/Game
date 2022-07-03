// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "PrimeiroGame/Personagens/Inimigo.h"

#include "InimigoPadrao1.generated.h"

UCLASS()
class PRIMEIROGAME_API AInimigoPadrao1 : public AInimigo
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInimigoPadrao1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleAnywhere)
	//class UWidgetComponent* HealthBar;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void TesteAiController();

private:
	bool bAndou = true;
	
};
