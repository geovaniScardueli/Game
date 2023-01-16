// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "Personagens/Protagonista/UI/PlayerWidget.h"

#include "PrimeiroGame.generated.h"

class AProtagonista;

UCLASS()
class PRIMEIROGAME_API APrimeiroGame : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "PlaySound")
	void PlaySounds(FVector Posicao, int32 Sound);

private:
	UPROPERTY()
	FVector LocalMovimentar;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameHudClass;

	UPROPERTY(VisibleInstanceOnly, Category = "UI")
	class UPlayerWidget* GameHud;

	UPROPERTY()
	AProtagonista* Player;

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
	void AtualizarVidaPlayer(int32 Value) { GameHud->AtualizarVida(Value); }

	UFUNCTION()
	void AtualizarEquilibrioPlayer(int32 Value) { GameHud->AtualizarEquilibrio(Value); }

	UFUNCTION()
	void PlaySoundsWord(FVector Posicao, int32 Sound) { PlaySounds(Posicao, Sound); }
	
	UFUNCTION()
	AProtagonista* GetPlayer() { return Player; }
};
