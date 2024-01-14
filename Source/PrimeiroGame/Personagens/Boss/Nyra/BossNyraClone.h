// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
#include "BossNyraClone.generated.h"

class APathToPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloneAction, FName, Action);

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

	UPROPERTY()
	class ABossAIController* BossAIControlle;

	UFUNCTION(BlueprintImplementableEvent, Category = "RunSpecialAtack")
	void RunSpecialAtack();

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackFinished();

public:	

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
	FOnCloneAction OnCloneAction;

	UFUNCTION()
	void Atack(const FName CloneAttack);

	UFUNCTION()
	void Grab();

	UFUNCTION()
	void CloseAttack(const FName Val);

	UFUNCTION()
	void DestroyClone(const bool Fire = true);

	UFUNCTION()
	void GrabEnd();

	UFUNCTION()
	void GrabStart();

	virtual void StartAttack(const FName IdAttack) override;
	virtual void StopAttack() override;
	virtual void ConectAttack(const FName Description) override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	APathToPlayer* PathPlayer = nullptr;

	UPROPERTY()
	FTimerHandle TimerHandlerDestroye;

	UPROPERTY()
	FTimerHandle TimerHandlertarget;

	UFUNCTION()
	void RealyDestroy();

	UFUNCTION()
	void TargetPlayer();

	enum CloneMontages
	{
		Discharge, Attack, Grabb
	};

};
