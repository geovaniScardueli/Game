// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bIsdead = false;

public:
	UFUNCTION()
	bool IsDead() { return bIsdead; }

	UFUNCTION()
	void CharacterIsDead();

	UFUNCTION()
	virtual void SetClosePlayer(bool Val) {};
	
};
