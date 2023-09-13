// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Damage.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UDamage : public UObject
{
	GENERATED_BODY()
	
	public:
		UFUNCTION()
		static int32 CalcaleteHealth(const int32 Power, const int32 Defense, const int32 AddPower = 0);
		UFUNCTION()
		static int32 CalcaletePosture(const int32 Power, const int32 Defense, const int32 AddDefense = 0);
};
