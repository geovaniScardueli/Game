// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BossService.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UBossService : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	
	UBossService(const FObjectInitializer& ObjectInitializer);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
