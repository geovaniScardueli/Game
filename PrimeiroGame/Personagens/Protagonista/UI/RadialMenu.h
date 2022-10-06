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

	private:
		//UPROPERTY()
		//FVector2D MetadeTela;
		//
		UPROPERTY()
		FIntPoint ViewPOrtSize;

	protected:
		UPROPERTY(meta = (BindWidget))
		class USizeBox* PainelPrincipal;

		UPROPERTY(meta = (BindWidget))
		class USizeBox* PainelArrow;

		UPROPERTY(meta = (BindWidget))
		class UImage* AimArrow;

		UPROPERTY(meta = (BindWidget))
		class UImage* SelectedImage;
	
	public:
		virtual void NativeConstruct() override;

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D MetadeTela;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Bla")
		FVector2D Vetor1;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "Bla")
		FVector2D Vetor2;

		UFUNCTION()
		void PosicaoMouseAtual(APlayerController* PlayerController);

		UFUNCTION()
		void CentralizeCursor(APlayerController* PlayerController);
};
