// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InimigoPadraoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API UInimigoPadraoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* VidaAtual = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* StaminaBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* StaminaAtual = nullptr;

public:

	UFUNCTION()
	void AtualizarVida(int32 Atual);

	UFUNCTION()
	void AtualizaStamina(float Val);

	UFUNCTION()
	void Changevisibility(const bool Val);
	
};
