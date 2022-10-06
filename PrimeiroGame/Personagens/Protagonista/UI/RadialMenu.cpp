// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URadialMenu::NativeConstruct()
{
	//https://www.youtube.com/watch?v=LEuHXecOpKQ&ab_channel=DraftShape-PyroDev
	//seleção em circulao wheel
	FVector2D BoxPrincipal = UWidgetLayoutLibrary::SlotAsCanvasSlot(Cast<UWidget>(PainelPrincipal))->GetPosition();
	//MetadeTela = BoxPrincipal +FVector2D(250, 250);

	//FVector2D Bla = USlateBlueprintLibrary::LocalToAbsolute(PainelArrow->GetCachedGeometry(), FVector2D(0.f, 0.f));
	FVector2D Bla = UWidgetLayoutLibrary::SlotAsCanvasSlot(Cast<UWidget>(PainelArrow))->GetPosition();
	

	AimArrow->GetDynamicMaterial();
	TArray<FIntPoint> Resolucao;//???
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolucao);

	ViewPOrtSize = Resolucao[Resolucao.Num() - 1];

	//AimArrow->SetVisibility(ESlateVisibility::Hidden);
}

void URadialMenu::PosicaoMouseAtual(APlayerController* PlayerController)
{
	//----------------------------------------------------------------------
	//forma 1 de raidial menu precis adicionar o +90 e fica meio estranho o algulo
	//caso precise trocar para essa forma

	//float PositionX = 1.f;
	//float PositionY = 2.f;
	//UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, PositionX, PositionY);
	////FVector2D PosicaoTela = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	//float rotacao = UKismetMathLibrary::FindLookAtRotation(FVector(MetadeTela.X, MetadeTela.Y, 0), FVector(PositionX, PositionY, 0)).Yaw;
	//UE_LOG(LogTemp, Warning, TEXT("rotacao %f"), rotacao);
	//AimArrow->SetRenderTransformAngle(AnglePosition + 90);
	//---------------------------------------------------------------

	///tentativa 2 de radial menu
	

	//float ViewPortX = (float)(MetadeTela.X / 2);
	//float ViewPortY = (float)(MetadeTela.Y / 2);
	////UE_LOG(LogTemp, Warning, TEXT("ViewPortX %f, ViewPortY %f"), ViewPortX, ViewPortY);
	//FVector2D ViePortSizeVector = FVector2D(ViewPortX, ViewPortY);
	//
	//Vetor1 = PosicaoTela - ViePortSizeVector;
	//Vetor2 = FVector2D(ViewPortX, 0.f) - ViePortSizeVector;
	//
	//UE_LOG(LogTemp, Warning, TEXT("Vetor1 %f, Vetor1 %f, Vetor2 %f, Vetor2 %f"), Vetor1.X, Vetor1.Y, Vetor2.X, Vetor2.Y);
	//
	//UKismetMathLibrary::Normalize2D(Vetor1);
	//UKismetMathLibrary::Normalize2D(Vetor2);
	//
	//double Dot = UKismetMathLibrary::DotProduct2D(Vetor1, Vetor2);
	//double TambemNaoSei = UKismetMathLibrary::DegAcos(Dot);
	//
	//float NaoSei = (PosicaoTela.X - ViewPortX) >= 0 ? 1.f : -1.f;
	//
	//double rotacao = TambemNaoSei * NaoSei;
	//
	//if (rotacao >= 0 && rotacao <= 60)
	//{
	//	SelectedImage->SetRenderTransformAngle(0.f);
	//}
	//else if (rotacao > 60 && rotacao <= 120)
	//{
	//	SelectedImage->SetRenderTransformAngle(60);
	//}
	//else if (rotacao > 120 && rotacao <= 180)
	//{
	//	SelectedImage->SetRenderTransformAngle(120);
	//}
	//else if (rotacao >= -60 && rotacao < 0)
	//{
	//	SelectedImage->SetRenderTransformAngle(-60.f);
	//}
	//else if (rotacao >= -120 && rotacao < -60)
	//{
	//	SelectedImage->SetRenderTransformAngle(-120.f);
	//}
	//else if (rotacao >= -180 && rotacao < -120)
	//{
	//	SelectedImage->SetRenderTransformAngle(-180.f);
	//}

	//------------------------------------------------------------

	///tentativa 3 de radial menu
	FVector2D MousePosicao = UWidgetLayoutLibrary::GetMousePositionOnPlatform();
	FVector2D PixelPosition;
	FVector2D ViewPortPosition;
	USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), MousePosicao, PixelPosition, ViewPortPosition);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	FVector2D Subtracao = (ViewportSize / 2) - PixelPosition;
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

	UE_LOG(LogTemp, Warning, TEXT("rotacao %f"), rotacao);
}

void URadialMenu::CentralizeCursor(APlayerController* PlayerController)
{	
	FVector2D Tela = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	//581.059 Y=362
	
	//UE_LOG(LogTemp, Warning, TEXT("vieportX %f, vieporty %f"), x, y);
	//FGeometry Temp = PainelPrincipal->GetCachedGeometry();

	//FVector2D Temp2 = FVector2D(USlateBlueprintLibrary::GetLocalSize(Temp).X + USlateBlueprintLibrary::GetLocalTopLeft(Temp).X, USlateBlueprintLibrary::GetLocalTopLeft(Temp).Y);
	//UE_LOG(LogTemp, Warning, TEXT("Temp2 %f, Temp2 %f"), Temp2.X, Temp2.Y);
	int32 Temp1;
	int32 	Temp2;
	PlayerController->GetViewportSize(Temp1, Temp2);
	MetadeTela = FVector2D(Temp1, Temp2);
	PlayerController->SetMouseLocation((Temp1 /2), Temp2/2);
}
