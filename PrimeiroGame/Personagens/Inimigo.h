// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "PrimeiroGame/PrimeiroGame.h"

#include "Inimigo.generated.h"

UCLASS()
class PRIMEIROGAME_API AInimigo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInimigo();

private:
	bool bCanMove = true;

	UFUNCTION()
	void DisableInDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	int VidaAtual = 100;
	UPROPERTY()
	float Stamina = 0.f;
	UPROPERTY()
	int BlockAtack = 10;
	UPROPERTY()
	bool bSeePlayer = false;
	UPROPERTY()
	APrimeiroGame* GameMode;

	UPROPERTY(BlueprintReadOnly, Category="Execution")
	float MeshValue = 0.f;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthBar;

	UPROPERTY(EditAnywhere, Category="Anim Montage")
	UAnimMontage* Die;

	UPROPERTY(EditAnywhere, Category="Anim Montage")
	UAnimMontage* Atack;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	TArray<UAnimMontage*> Montages;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* TakeHitAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* TakeExecution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UCapsuleComponent* CapsuleWeapon;

	UFUNCTION(BlueprintImplementableEvent, Category = "Mover")
	void ComecarMover();

	UFUNCTION(BlueprintCallable, Category = "Mover")
	void ComecarMoverUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Mover")
	void ComecarMoverFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ComecarAMover();

	UFUNCTION()
	void AtackPlayer();

	UFUNCTION()
	void EnableDisabelOverlapWeapon(bool Val) { CapsuleWeapon->SetGenerateOverlapEvents(Val); }

	UFUNCTION()
	void ParryAnimation(float Val);

	UFUNCTION()
	void ChangeExecuteMode(float Val) { MeshValue = Val; OnExecuteMode(); }
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Execution")
	void OnExecuteMode();

	UFUNCTION()
	void OnTakeExecution(bool bIsInFront, AActor* Player);

	UFUNCTION()
	float GetStamina() { return Stamina; }

	UFUNCTION()
	bool IsSeePlayer() { return bSeePlayer; }

	UFUNCTION()
	void SetSeePlayer(bool Val) { bSeePlayer = Val; }

	UFUNCTION()
	void ChangeStamina(float Val);

	UFUNCTION()
	int TakeHit(AActor* OtherActor, int Damage, float DamageStamina, int animation = 0);

	UFUNCTION()
	void TakeDamage(int Val);

	UFUNCTION()
	void ChangeBlackboarValue(const FName Description, bool Val);

	UFUNCTION()
	bool GetBlackboarValue(const FName Description);

	UFUNCTION()
	AActor* GetPlayer() { return  GameMode->GetPlayer(); }

	UFUNCTION()
	void ChangeVisibilityUI(bool Val) { HealthBar->SetVisibility(Val); } //IsSeePlayer() && !Val ? NULL : HealthBar->SetVisibility(Val); }

	UFUNCTION()
	bool IsDead();

	UFUNCTION()
	void SensePlayer();

	UFUNCTION()
	void SpecialAtack(FVector Location);

};
