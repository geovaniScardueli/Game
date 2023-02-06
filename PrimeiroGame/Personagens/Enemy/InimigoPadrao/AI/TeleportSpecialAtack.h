// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TeleportSpecialAtack.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UTeleportSpecialAtack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTeleportSpecialAtack(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
