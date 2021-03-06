// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Windows/AllowWindowsPlatformTypes.h"


#include "Protagonista.generated.h"

class UProtagonistaAninInstance;
UCLASS()
class PRIMEIROGAME_API AProtagonista : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProtagonista();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	class USpringArmComponent* SpringArmCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	class UArrowComponent* Flecha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	class UBoxComponent* BoxTarget; 
	
	UPROPERTY(EditAnywhere, Category="Anim Montage hit")
	UAnimMontage* Atack;

	UPROPERTY(EditAnywhere, Category="Anim Montage hit")
	UAnimMontage* InicioParry;

	UPROPERTY(EditAnywhere, Category="Anim Montage hit")
	UAnimMontage* TakeHit;

	UPROPERTY(EditAnywhere, Category="Anim Montage hit")
	UAnimMontage* Execution;

	UPROPERTY(EditAnywhere, Category="Anim Montage hit")
	UAnimMontage* Moviment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UCapsuleComponent* CapsuleWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UCapsuleComponent* CapsuleParry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UBoxComponent* BoxExecution;

	UFUNCTION(BlueprintImplementableEvent, Category = "Approach")
	void ApproachEnemy();

	UFUNCTION(BlueprintCallable, Category = "Approach")
	bool ApproachEnemyUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Approach")
	void ApproachEnemyFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "MoveCamera")
	void MoveCameraTeleport();

	UFUNCTION(BlueprintCallable, Category = "MoveCamera")
	bool MoveCameraTeleportUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "MoveCamera")
	void MoveCameraTeleportFinished();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	bool bIsInParryAnim = false;

	UPROPERTY()
	bool bIsInParryTime = false;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void EnableDisabelOverlapWeapon(bool Val);

	UFUNCTION()
	bool IsInDefensePosition() { return bIsInDefensePosition; }

	UFUNCTION()
	void OnEnterDefensePosition();

	UFUNCTION()
	void EnableDisableOverlapMesh(bool Val) { GetMesh()->SetGenerateOverlapEvents(Val); }

	UFUNCTION()
	bool ValidarEnemigoCampoVisao(AActor* Actor);

	UFUNCTION()
	void ResetPerfectPArry() { bIsPerfectParry = false; }

	UFUNCTION()
	void ChangeAtackStatus(bool Val) { bIsAtackEnable = Val; }

	UFUNCTION()
	void ChangeMovementStatus(bool Val) { bIsMovementEnable = Val; }

	UFUNCTION()
	void ChangeRotator(bool Val) { bIsRotateEnable = Val; }

	UFUNCTION()
	void ChangeIndexAtackSequence() { IndexAtack++; if (IndexAtack > 1) ResetIndexAtackSequence(); }

	UFUNCTION()
	void ResetIndexAtackSequence() { IndexAtack = 0; }

	UFUNCTION()
	void ArriveTeleportMoviment();

	UFUNCTION()
	void MoveCamera() { MoveCameraTeleport(); };

	UFUNCTION()
	void EnabledInTeleportMoviment() { bIsInTeleportMoviment = false; }

private:
	UPROPERTY()
	class UProtagonistaAninInstance* ProtagonistaAninInstance;
	UPROPERTY()
	class AInimigo* InimigoFocado;
	UPROPERTY()
	class AInimigo* InimigoCampoVisao;
	UPROPERTY()
	bool bFocoEnemigo = false;
	UPROPERTY()
	bool bIsInDefensePosition = false;
	UPROPERTY()
	int VidaAtual = 100;
	UPROPERTY()
	float AnguloInimigo = -1.f;
	UPROPERTY()
	bool bIsPerfectParry = false;
	UPROPERTY()
	bool bIsMovementEnable = true;
	UPROPERTY()
	bool bIsRotateEnable = true;
	UPROPERTY()
	bool bIsAtackEnable = true;
	UPROPERTY()
	int IndexAtack = 0;
	UPROPERTY()
	bool bIsInTeleportMoviment = false;

	UFUNCTION()
	void MoveForward(float Value);
	
	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void Atacar();

	UFUNCTION()
	void Focar();

	UFUNCTION()
	void TeleportMoviment();

	UFUNCTION()
	void DesativarOverlap();

	UFUNCTION()
	void MoverMousePitch(float Val);

	UFUNCTION()
	void MoverMouseYaw(float Val);

	UFUNCTION()
	void ParryAtack();

	UFUNCTION()
	void StopParry();

	UFUNCTION()
	void StartSequenceAtack();

	UFUNCTION()
	void OnParryMoment(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEnterExecutionMode(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEXitExecutionMode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAtack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};


