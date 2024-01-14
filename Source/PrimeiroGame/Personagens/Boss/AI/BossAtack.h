// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BossAtack.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UBossAtack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBossAtack(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
