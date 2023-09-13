// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossNyraClone.generated.h"

class APathToPlayer;

UCLASS()
class PRIMEIROGAME_API ABossNyraClone : public ACharacter
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
	UAnimMontage* CloneMontage;

	UFUNCTION(BlueprintImplementableEvent, Category = "RunSpecialAtack")
	void RunSpecialAtack();

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackFinished();

public:	

	UFUNCTION()
	void Atack(APrimeiroGame* GameMode);

	UFUNCTION()
	void KickAtack();

	UFUNCTION()
	void DestroyClone();

private:
	UPROPERTY()
	APathToPlayer* PathPlayer = nullptr;

	UPROPERTY()
	APrimeiroGame* GameMode;

	UPROPERTY()
	FTimerHandle TimerHandlerDestroye;

	UFUNCTION()
	void RealyDestroy();

};
