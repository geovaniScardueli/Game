// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "InimigoPadrao/Head/DefaulEnemyHead.h"

#include "Inimigo.generated.h"


UCLASS()
class PRIMEIROGAME_API AInimigo : public ACharacter
{

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInimigo();

	enum AnimMontages
	{
		EHit, EAtack, EParry, EDefense, EDash, EDie, EExecution, ESpecialAtack, ECreateClone, EGrab, EComboAtack, ETeleport
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	int32 VidaAtual = 100;
	UPROPERTY()
	float Stamina = 0.f;
	UPROPERTY()
	int32 BlockAtack = 10;
	UPROPERTY()
	int32 AtackSequence = 0;
	UPROPERTY()
	bool bSeePlayer = false;
	UPROPERTY()
	APrimeiroGame* GameMode = nullptr;
	UPROPERTY()
	bool bCanMove = true;
	UPROPERTY()
	bool bUnstoppableAttack = false;
	UPROPERTY()
	class AEnemyAIController* EnemyAIControlle;

	UPROPERTY(BlueprintReadWrite, Category="Execution")
	float MeshValue = 0.f;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* EnemyWidgetComp;

	UPROPERTY(VisibleAnywhere)
	class UInimigoPadraoWidget* EnemyDefaultWidget;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int32 AtackPower = 10;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int32 Defense = 10;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int32 DefensePosture = 10;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<UUserWidget> EnemyDefaultWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TArray<UAnimMontage*> Montages;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<ADefaulEnemyHead> HeadClass;

	UFUNCTION(BlueprintImplementableEvent, Category = "Mover")
	void ComecarMover();

	UFUNCTION(BlueprintCallable, Category = "Mover")
	void ComecarMoverUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Mover")
	void ComecarMoverFinished();

	UFUNCTION()
	virtual void DisableInDead();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ComecarAMover();

	UFUNCTION()
	virtual void AtackPlayer() {};

	UFUNCTION()
	virtual void EnableDisabelOverlapWeapon(bool Val) {};

	UFUNCTION()
	virtual void PhysicsWeapon() {};

	UFUNCTION()
	virtual void ParryAnimation(float Val, FVector LocationPlayer);

	UFUNCTION()
	virtual void ThrowGrab() {};

	UFUNCTION()
	void ChangeExecuteMode(float Val) { MeshValue = Val; OnExecuteMode(); }
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Execution")
	void OnExecuteMode();

	UFUNCTION()
	void OnTakeExecution(bool bIsInFront, AActor* Player);

	UFUNCTION()
	FORCEINLINE float GetStamina() { return Stamina; }

	UFUNCTION()
	FORCEINLINE int32 GetAtackPower() { return AtackPower; }

	UFUNCTION()
	FORCEINLINE int32 GetAtackSequence() { return AtackSequence; }

	UFUNCTION()
	FORCEINLINE bool IsSeePlayer() { return bSeePlayer; }

	UFUNCTION()
	void SetSeePlayer(bool Val) { bSeePlayer = Val; }

	UFUNCTION()
	void ChangeStamina(float Val);

	UFUNCTION()
	virtual int32 TakeHit(const int32 Power, const int32 Atack);

	UFUNCTION()
	virtual void EnableDisableOverBody(const bool Enable) {};

	UFUNCTION()
	void TakeExecutionPerfectParry(FVector FowardPlayer);

	UFUNCTION()
	void LoseHealth(int Val);

	UFUNCTION()
	virtual void ChangeBlackboarValue(const FName Description, bool Val) {};

	UFUNCTION()
	bool GetBlackboarValue(const FName Description);

	UFUNCTION()
	AProtagonista* GetPlayer() { return  GameMode->GetPlayer(); }

	UFUNCTION()
	void ChangeVisibilityUI(const bool Val);

	UFUNCTION()
	bool IsDead();

	UFUNCTION()
	virtual void SensePlayer() {};

	UFUNCTION()
	virtual void StartAttack() {};

	UFUNCTION()
	virtual void StopAttack() {};

	UFUNCTION()
	virtual void ResetAllStatus() {};

	UFUNCTION()
	void SpecialAtack(FVector Location);

	UFUNCTION()
	void Decapitated();

	UFUNCTION()
	void TesteTemp();

};
