// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/SlateBlueprintLibrary.h"

void URadialMenu::NativeConstruct()
{
	//seleção em circulao wheel
}

void URadialMenu::PosicaoMouseAtual(APlayerController* PlayerController)
{
	FVector2D PixelPosition;
	FVector2D ViewPortPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), UWidgetLayoutLibrary::GetMousePositionOnPlatform(), PixelPosition, ViewPortPosition);
	FVector2D ViewPortSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	FVector2D Subtracao = (ViewPortSize / 2) - PixelPosition;
	Subtracao.Normalize();
	bool MaiorZero = Subtracao.X > 0;
	FVector2D Temp = MaiorZero ? FVector2D(0.f, 1.f) : FVector2D(0.f, -1.f);
	double DotProducVetor = UKismetMathLibrary::DotProduct2D(Subtracao, Temp);
	float rotacao = (DotProducVetor * 90) + (MaiorZero ? 180.f : 0.f);

	if (rotacao >= -90.f && rotacao <= -44.f)
	{
		SelectedImage->SetRenderTransformAngle(0.f);
	}
	else if (rotacao > -45.f && rotacao <= 45.f)
	{
		SelectedImage->SetRenderTransformAngle(60);
	}
	else if (rotacao > 46.f && rotacao <= 90.f)
	{
		SelectedImage->SetRenderTransformAngle(120);
	}
	else if (rotacao >= 230.f && rotacao < 270.f)
	{
		SelectedImage->SetRenderTransformAngle(-60.f);
	}
	else if (rotacao >= 135.f && rotacao < 229.f)
	{
		SelectedImage->SetRenderTransformAngle(-120.f);
	}
	else if (rotacao >= 90.f && rotacao < 135.f)
	{
		SelectedImage->SetRenderTransformAngle(-180.f);
	}
}

void URadialMenu::CentralizeCursor(APlayerController* PlayerController)
{	
	int32 SizeX;
	int32 SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);
	PlayerController->SetMouseLocation((SizeX /2), SizeY /2);
}

float URadialMenu::GetSelectSpecialAtack()
{
	return SelectedImage->GetRenderTransformAngle();
}
