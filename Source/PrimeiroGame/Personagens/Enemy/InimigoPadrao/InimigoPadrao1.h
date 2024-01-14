// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PrimeiroGame/Personagens/Enemy/Inimigo.h"

#include "InimigoPadrao1.generated.h"

UCLASS()
class PRIMEIROGAME_API AInimigoPadrao1 : public AInimigo
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInimigoPadrao1();

protected:
	// Called when the game starts or when spawned
	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY()
	class AInimigoAIController* InimigoPadraoAIControlle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UCapsuleComponent* CapsuleWeapon;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<AActor> SwordClass;

	UPROPERTY(BlueprintReadWrite)
	class AActor* SwordBP;

	UFUNCTION(BlueprintImplementableEvent, Category = "Die")
	void EnablePhysicsWeapon();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void TesteAiController();

	virtual void EnableDisabelOverlapWeapon(bool Val) override { CapsuleWeapon->SetGenerateOverlapEvents(Val); }
	virtual void AtackPlayer() override;
	virtual void ChangeBlackboarValue(const FName Description, bool Val) override;
	virtual void SensePlayer() override;
	virtual void PhysicsWeapon() override;

private:
	bool bAndou = true;
	
};
