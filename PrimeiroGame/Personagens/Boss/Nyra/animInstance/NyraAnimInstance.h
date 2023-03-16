// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NyraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UNyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool GoToRunState;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Velocidade;
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
