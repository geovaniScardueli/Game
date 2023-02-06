// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Enemy/AI/EnemyAIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "InimigoAIController.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API AInimigoAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	AInimigoAIController();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PercebeuAtores3(AActor* Actor, FAIStimulus Stimulus);

	

};
