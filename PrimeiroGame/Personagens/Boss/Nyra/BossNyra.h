// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossNyra.generated.h"

class APrimeiroGame;
class APathToPlayer;

UCLASS()
class PRIMEIROGAME_API ABossNyra : public ACharacter
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

private:
	UPROPERTY()
	APathToPlayer* PathPlayer = nullptr;

	UPROPERTY()
	APrimeiroGame* GameMode = nullptr;

	UFUNCTION()
	void SpecialAtack();
};
