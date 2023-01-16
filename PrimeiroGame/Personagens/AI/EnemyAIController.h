// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void DisableBehaviorTree();

protected:

	UPROPERTY()
	class UAISenseConfig_Sight* sightConfig;
	
};
