// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialMenu.generated.h"

/**
 * 
 */
UCLASS()
class PRIMEIROGAME_API URadialMenu : public UUserWidget
{
	GENERATED_BODY()

	protected:

		UPROPERTY(meta = (BindWidget))
		class UImage* SelectedImage;
	
	public:
		virtual void NativeConstruct() override;

		UFUNCTION()
		void PosicaoMouseAtual(APlayerController* PlayerController);

		UFUNCTION()
		void CentralizeCursor(APlayerController* PlayerController);

		UFUNCTION()
		float GetSelectSpecialAtack();
};
