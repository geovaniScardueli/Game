// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "InimigoPadraoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UInimigoPadraoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* VidaAtual;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StaminaAtual;

public:
	UFUNCTION()
	void AtualizarVida(int Atual);

	UFUNCTION()
	void AtualizaStamina(float Val);
	
};
