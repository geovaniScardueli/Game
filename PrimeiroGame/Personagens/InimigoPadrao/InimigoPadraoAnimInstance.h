// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "InimigoPadraoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UInimigoPadraoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bIsdead = false;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool GoToRunState;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Velocidade;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Rotacao;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bCloseplayerSpace = false;

public:
	void CharacterIsDead();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void SetClosePlayer(bool Val);
};
