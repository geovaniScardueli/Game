// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
#include "BossNyraClone.generated.h"

class APathToPlayer;

UCLASS()
class PRIMEIROGAME_API ABossNyraClone : public AInimigo
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossNyraClone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<APathToPlayer> PathPlayerClass;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TArray<UAnimMontage*> CloneMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dependencia Fonte")
	class UCapsuleComponent* CapsuleWeapon;

	UFUNCTION(BlueprintImplementableEvent, Category = "RunSpecialAtack")
	void RunSpecialAtack();

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackFinished();

public:	

	UFUNCTION()
	void Atack();

	UFUNCTION()
	void CloseAttack(const FName Val);

	UFUNCTION()
	void DestroyClone();

	virtual void StartAttack() override;
	virtual void StopAttack() override;

private:
	UPROPERTY()
	APathToPlayer* PathPlayer = nullptr;

	UPROPERTY()
	FTimerHandle TimerHandlerDestroye;

	UFUNCTION()
	void RealyDestroy();

	enum CloneMontages
	{
		Discharge, Attack
	};

};
