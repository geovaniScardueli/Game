// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerItemNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerItemNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{

		Player->ApplyEfectItem(ItensTypeBla[IdSection]);

		//switch (IdSection)
		//{
		//	case EMontageItens::Type::Heal:
		//	{
		//		Player->ApplyEfectItem(FName("heal"));
		//		return;
		//	}
		//	default:
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("defal"));
		//	}
		//}
	}
}
