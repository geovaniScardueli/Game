// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* VidaAtual;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Equilibrio;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* EquilibrioAtual;

public:
	UFUNCTION()
	void AtualizarVida(int32 Atual);

	UFUNCTION()
	void AtualizarEquilibrio(int32 Atual);

};
