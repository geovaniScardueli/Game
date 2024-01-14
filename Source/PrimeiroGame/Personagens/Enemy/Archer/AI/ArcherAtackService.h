// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "ArcherAtackService.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UArcherAtackService : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UArcherAtackService(const FObjectInitializer& ObjectInitializer);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
