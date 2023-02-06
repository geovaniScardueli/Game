// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "ClosePlayerService.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UClosePlayerService : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:

	UClosePlayerService(const FObjectInitializer& ObjectInitializer);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
