// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Enemy/AI/EnemyAIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "ArcherAIController.generated.h"

/**
 * 
 */

UCLASS()
class PRIMEIROGAME_API AArcherAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	AArcherAIController();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void PercebeuActors(AActor* Actor, FAIStimulus Stimulus);
};
