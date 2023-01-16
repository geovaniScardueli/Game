// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProtagonistaAninInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UProtagonistaAninInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	APawn* Owner;

public:
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Velocidade;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	float Rotacao;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool Focar;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bIsInAir;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bIsInParry = false;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	bool bIsRunning = false;
	UPROPERTY(BlueprintReadOnly, Category = Generic)
	FRotator RotacaoAtack;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void ChangeFocar() { Focar = !Focar; }

	UFUNCTION()
	void SetRunState(bool Val) { bIsRunning = Val; }

	UFUNCTION()
	void SetIsInDefensePosition(bool Val) { bIsInParry = Val; }

	UFUNCTION()
	void SetInAir(bool Val) { bIsInAir = Val; }

	UFUNCTION()
	void SetRotationAtack(FRotator Val) { RotacaoAtack = Val; }
};
