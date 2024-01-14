// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerWidget::AtualizarVida(int32 Atual)
{

	HealthBar->SetPercent(Atual / 100.f);
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	VidaAtual->SetText(FText::AsNumber(Atual, &Opts));
}

void UPlayerWidget::AtualizarEquilibrio(int32 Atual)
{
	Equilibrio->SetPercent(Atual / 100.f);
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	EquilibrioAtual->SetText(FText::AsNumber(Atual, &Opts));
}
