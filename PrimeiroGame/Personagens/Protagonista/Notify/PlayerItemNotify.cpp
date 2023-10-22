// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerItemNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"

void UPlayerItemNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		//switch (IdSection)
		//{
		//	case Type::Heal:
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("heal"));
		//	}
		//	default:
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("defal"));
		//	}
		//}
	}
}
