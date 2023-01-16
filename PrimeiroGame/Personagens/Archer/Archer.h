// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimeiroGame/Personagens/Inimigo.h"
#include "Archer.generated.h"

/**
 * 
 */

class AArrow;

UCLASS()
class PRIMEIROGAME_API AArcher : public AInimigo
{
	GENERATED_BODY()

public:

	virtual void AtackPlayer() override;

	UFUNCTION()
	void VisibilityArrow(const bool bShow);

	UFUNCTION()
	void ShootArrow();

	virtual void ChangeBlackboarValue(const FName Description, bool Val) override;
	virtual void SensePlayer() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dependencia Fonte")
	TSubclassOf<AArrow> ArrowClass;

private:
	UPROPERTY()
	class AArrow* Arrow;
	UPROPERTY()
	class AArcherAIController* ArcherAIController;
};
