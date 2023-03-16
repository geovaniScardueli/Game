// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
#include "BossNyra.generated.h"

class APrimeiroGame;
class APathToPlayer;

UENUM()
enum BossMontages
{
	BHit, BAtack, BParry
};

UCLASS()
class PRIMEIROGAME_API ABossNyra : public AInimigo
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossNyra();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<APathToPlayer> PathPlayerClass;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TArray<UAnimMontage*> ArrayMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dependencia Fonte")
	class UCapsuleComponent* CapsuleWeapon;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	float Damage = 10;

	UPROPERTY()
	class ABossAIController* BossAIControlle;

	UFUNCTION(BlueprintImplementableEvent, Category = "RunSpecialAtack")
	void RunSpecialAtack();

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category = "RunSpecialAtack")
	void RunSpecialAtackFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TesteMethod();
	
	UFUNCTION()
	void BossTakeHit(const int16 DamageTake, const float StaminaDamage, int16 animation);

	UFUNCTION()
	void EnableDisableOverlapWeapon1(const bool Enable);

	UFUNCTION()
	void EnableDisableOverlapWeapon2(const bool Enable);

	UFUNCTION()
	void ResetAllStatus();

	virtual void AtackPlayer() override;
	virtual void ChangeBlackboarValue(const FName Description, bool Val) override;
	virtual void ParryAnimation(float Val, FVector LocationPlayer) override;

private:
	UPROPERTY()
	APathToPlayer* PathPlayer = nullptr;

	UPROPERTY()
	TArray<FName> Hits = {TEXT("Hit1"),TEXT("Hit2") ,TEXT("Hit3") };

	UFUNCTION()
	void BossSpecialAtack();

	UFUNCTION()
	void EnableDisableOverBody(const bool Enable);

};
