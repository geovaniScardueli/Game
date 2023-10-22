// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"
#include "BossNyra.generated.h"

class APrimeiroGame;
class APathToPlayer;
class UNyraAnimInstance;
class ABossNyraClone;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dependencia Fonte")
	class UCapsuleComponent* CapsuleWeapon;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	float Damage = 10;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<AActor> Eye;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<ABossNyraClone> CloneClass;

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
	void EnableDisableOverlapWeapon1(const bool Enable);

	UFUNCTION()
	void EnableDisableOverlapWeapon2(const bool Enable);

	UFUNCTION()
	void ResetStatus(const FName Status);

	UFUNCTION()
	void CheckAfterParry();

	UFUNCTION()
	FName GetMontageSection();

	UFUNCTION()
	void ChangeCapsuleWeapon(const FName Socket);

	UFUNCTION()
	int32 CheckWhatToDo();

	UFUNCTION()
	void CreateClone();

	UFUNCTION()
	void DischargeSingleClone();

	UFUNCTION()
	void Grab();

	UFUNCTION()
	void CreateCloneKick();

	UFUNCTION()
	void CheckToTeleport();

	UFUNCTION()
	void StartTeleport();

	virtual void ThrowGrab() override;
	virtual void AtackPlayer() override;
	virtual void ChangeBlackboarValue(const FName Description, bool Val) override;
	virtual void ParryAnimation(float Val, FVector LocationPlayer) override;
	virtual int32 TakeHit(const int32 Power, const int32 Atack) override;
	virtual void EnableDisableOverBody(const bool Enable) override;
	virtual void StartAttack() override;
	virtual void StopAttack() override;
	virtual void ResetAllStatus() override;

private:
	UPROPERTY()
	APathToPlayer* PathPlayer = nullptr;

	UPROPERTY()
	ABossNyraClone* Clone = nullptr;

	UPROPERTY()
	class UNyraAnimInstance* NyraAnimInstance;

	UPROPERTY()
	TArray<FName> Hits = {TEXT("Hit1"),TEXT("Hit2") ,TEXT("Hit3"), TEXT("Hit3") };

	UPROPERTY()
	TArray<FName> DefenseAnim = { FName("Defense1"),FName("Defense2") ,FName("Defense3"), FName("Defense4") };

	UPROPERTY()
	int32 RNumber;

	UPROPERTY()
	AActor* LeftEye;

	UPROPERTY()
	AActor* RightEye;

	UPROPERTY()
	int32 CloneAtack = 1;

	UPROPERTY()
	FTimerHandle TimerEnableOverlap;

	UPROPERTY()
	bool bFirstFace = true;

	UFUNCTION()
	void BossSpecialAtack();

	UFUNCTION()
	void EnableOverlap();

	UFUNCTION()
	FVector GetLocationTeleport(const int16 Val);

};
