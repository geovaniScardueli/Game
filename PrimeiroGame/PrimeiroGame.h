// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "Personagens/Protagonista/UI/PlayerWidget.h"

#include "PrimeiroGame.generated.h"

UCLASS()
class PRIMEIROGAME_API APrimeiroGame : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "PlaySound")
	void PlaySounds(FVector Posicao, int Sound);

private:
	UPROPERTY()
	FVector LocalMovimentar;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameHudClass;

	UPROPERTY(VisibleInstanceOnly, Category = "UI")
	class UPlayerWidget* GameHud;
	UPROPERTY()
	AActor* Player;

public:
	UFUNCTION()
	void SetLocalMovimentacao(FVector Val) { LocalMovimentar = Val; }

	UFUNCTION()
	FVector GetLocalMovimentacao() { return LocalMovimentar; }

	UFUNCTION()
	void MoverLocalMovimentacao();

	UFUNCTION()
	void ResetLocalMovimentacao() { LocalMovimentar.Set(0.f, 0.f, 0.f); }

	UFUNCTION()
	void AtualizarVidaPlayer(int Value) { GameHud->AtualizarVida(Value); }

	UFUNCTION()
	void PlaySoundsWord(FVector Posicao, int Sound) { PlaySounds(Posicao, Sound); }
	
	UFUNCTION()
	AActor* GetPlayer() { return Player; }
};
