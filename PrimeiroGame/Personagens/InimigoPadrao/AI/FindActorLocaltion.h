// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindActorLocaltion.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UFindActorLocaltion : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UFindActorLocaltion(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool bAndou = true;
};
