// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PrimeiroGame/Personagens/EnemyAnimInstance.h"
#include "InimigoPadraoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UInimigoPadraoAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool GoToRunState;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Velocidade;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Rotacao;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bCloseplayerSpace = false;

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void SetClosePlayer(bool Val) override;

};
