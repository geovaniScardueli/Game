// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AtackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UAtackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UAtackPlayer(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
