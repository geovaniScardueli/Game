// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SeguirPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API USeguirPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USeguirPlayer(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
