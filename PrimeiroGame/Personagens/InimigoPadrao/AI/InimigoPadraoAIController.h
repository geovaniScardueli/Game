// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "InimigoPadraoAIController.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API AInimigoPadraoAIController : public AAIController
{
	GENERATED_BODY()

public:
	AInimigoPadraoAIController();

	UFUNCTION()
	void DisableBehaviorTree();

protected:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void PercebeuAtores3(AActor* Actor, FAIStimulus Stimulus);

	class UAISenseConfig_Sight* sightConfig;

};
