// Fill out your copyright notice in the Description page of Project Settings.


#include "AirAtackNotify.h"
#include "PrimeiroGame/Personagens/Protagonista/Protagonista.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UAirAtackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AProtagonista* Player = Cast<AProtagonista>(MeshComp->GetOwner()))
	{
		//FVector Frente = UKismetMathLibrary::GetForwardVector(Player->GetActorRotation());
		FVector Frente = Player->GetActorForwardVector();

		//UE_LOG(LogTemp, Warning, TEXT("f: %f, f: %f, f: %f, fa: %f, fa: %f, fa: %f"), Frente.X, Frente.Y, Frente.Z, FrenteDois.X, FrenteDois.Y, FrenteDois.Z);

		FVector DiagonalFrente = Player->GetActorLocation() + (Frente * 3000.f);
		DiagonalFrente.Z = -900.f;

		const TArray<AActor*> Ignore;
		FHitResult Bateu;

		UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			Player->GetMesh()->GetSocketLocation(FName("DEF-hand_R")),
				DiagonalFrente, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, Ignore, EDrawDebugTrace::ForDuration,
			Bateu, true);
		
		Player->AirTeleporPosition(Bateu);
	}
}
