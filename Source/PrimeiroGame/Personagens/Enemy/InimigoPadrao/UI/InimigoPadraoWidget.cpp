// Fill out your copyright notice in the Description page of Project Settings.


#include "InimigoPadraoWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UInimigoPadraoWidget::AtualizarVida(int Atual)
{
	VidaAtual->SetText(FText::AsNumber(Atual));
	HealthBar->SetPercent(Atual / 100.f);
	//nao me lembro pra q isso serve
	//FNumberFormattingOptions Opts;
	//Opts.SetMaximumFractionalDigits(0);
	//VidaAtual->SetText(FText::AsNumber(Atual, &Opts));
}

void UInimigoPadraoWidget::AtualizaStamina(float Val)
{
	StaminaAtual->SetText(FText::AsNumber(Val));
	StaminaBar->SetPercent(Val / 100.f);
}

void UInimigoPadraoWidget::Changevisibility(const bool Val)
{
	
}
