// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Windows/AllowWindowsPlatformTypes.h"

#include "Protagonista.generated.h"

class UProtagonistaAninInstance;
class UPhysicalAnimationComponent;
UCLASS()
class PRIMEIROGAME_API AProtagonista : public ACharacter
{
	enum AnimMontages
	{
		EPAtack, EPDefense, EPHit, EPExecution, EPMoviment, EPSpecialAtack, EPAirAtack, EPParyAtack, EPGrab, EStandUp, EHeal
	};

	enum States
	{
		sIdleMov, sHit, sStartAttack, sAttack, sAttackEnd, sJump, sDash, sStartParry, sParry, sEndParry, sDefense, sItem, sTeleport, sLock, sGrab
	};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UPhysicalAnimationComponent* PhysicalAnimationComponent;
	
	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	TArray<UAnimMontage*> ArrayMontage;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int16 AtackPower = 10;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int16 Defense = 10;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int16 DefensePosture = 10;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	int16 ParryPosture = 10;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	TSubclassOf<AActor> SwordBlueprintClass;

	UPROPERTY(EditAnywhere, Category = "dependencia fonte")
	TSubclassOf<UUserWidget> RadialWheelClass;

	UPROPERTY(VisibleInstanceOnly, Category = "dependencia fonte")
	class URadialMenu* RadialWheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UCapsuleComponent* CapsuleWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UCapsuleComponent* CapsuleParry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	class UBoxComponent* BoxExecution;

	//---------------------------------deletar

	UFUNCTION(BlueprintImplementableEvent, Category = "Approach")
	void ApproachEnemy(FVector Inimigo);

	UFUNCTION(BlueprintCallable, Category = "Approach")
	bool ApproachEnemyUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Approach")
	void ApproachEnemyFinished();

	UFUNCTION(BlueprintCallable, Category = "Teste")
	void TesteComBlueprint(AActor* Boss);

	//---------------------------------

	UFUNCTION(BlueprintCallable, Category = "Approach")
	void ExecutionEnemy();

	UFUNCTION(BlueprintImplementableEvent, Category = "MoveCamera")
	void MoveCameraTeleport();

	UFUNCTION(BlueprintCallable, Category = "MoveCamera")
	bool MoveCameraTeleportUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "MoveCamera")
	void MoveCameraTeleportFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "MoveCamera")
	void AirMoveCameraTeleport(FTransform CameraPosition);

	UFUNCTION(BlueprintCallable, Category = "MoveCamera")
	void AirMoveCameraTeleportFinished(FVector Localizacao);

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
	void EnableDisableOverlapMesh(bool Val);

	UFUNCTION()
	bool ValidarEnemigoCampoVisao(AActor* Actor);

	UFUNCTION()
	void ChangeIndexAtackSequence();

	UFUNCTION()
	void ResetIndexAtackSequence();

	UFUNCTION()
	void ArriveTeleportMoviment();

	UFUNCTION()
	void MoveCamera();

	UFUNCTION()
	void VerifyEnemyLockIsDead();

	UFUNCTION()
	void ResetStatus(const FName Status);

	UFUNCTION()
	void SetRunState(bool Val);

	UFUNCTION()
	void StopDash();

	UFUNCTION()
	void DashNotifyEnd();

	UFUNCTION()
	void DestroyActorSword() { if (SwordBlueprint) SwordBlueprint->Destroy(); };

	UFUNCTION()
	void AirTeleporPosition(FHitResult Bateu);

	UFUNCTION()
	void LevouPorrada(const bool Front);

	UFUNCTION()
	void LaunchPlayer(const FVector Direction);

	UFUNCTION()
	void ApplyEfectItem(const FName Item);

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	void StopAttack();

	UFUNCTION()
	void Grab(const FName Name);

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
	int16 VidaAtual = 100;
	UPROPERTY()
	int16 MaxHealth = 100;
	UPROPERTY()
	int16 EquilibrioAtual = 0;
	UPROPERTY()
	float AnguloInimigo = -1.f;
	//UPROPERTY()
	//bool bIsPerfectParry = false;
	//UPROPERTY()
	//bool bIsMovementEnable = true;

	UPROPERTY()
	int16 StatePlayer;

	//UPROPERTY()
	//bool bParryDashEnable = true;
	//UPROPERTY()
	//bool bIsRotateEnable = true;
	//UPROPERTY()
	//bool bIsAtackEnable = true;
	//UPROPERTY()
	//bool bIsInTeleportMoviment = false;


	UPROPERTY()
	int16 IndexAtack = 0;
	
	UPROPERTY()
	bool bIsFaceDown = false;
	UPROPERTY()
	FTimerHandle TimerHandlerBalance;
	UPROPERTY()
	FTimerHandle TimerHandlerWheelSpecial;
	UPROPERTY()
	FTimerHandle TimerHandlerGetUp;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	float IndexSelectSpecialAtack;
	UPROPERTY()
	AActor* SwordBlueprint;
	UPROPERTY()
	FVector CameraEnd;
	UPROPERTY()
	class APrimeiroGame* GameMode;
	UPROPERTY()
	TMap<int16, FName> AtackSequences = {
		{0, TEXT("FirstAtack")},
		{1, TEXT("SecondAtack")},
		{2, TEXT("ThirdAtack")},
		{3, TEXT("FourAtack")},
		{4, TEXT("AtackPerfectParry")}
	};
	UPROPERTY()
	TMap<int16, FName> HitSequences = {
		{0, TEXT("Hit1")},
		{1, TEXT("Hit2")}
	};

	UFUNCTION()
	void MoveForward(float Value);
	
	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void Atacar();

	UFUNCTION()
	void PlayerJump();

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
	void Dash();

	UFUNCTION()
	void ControlBalance(int32 Val);

	UFUNCTION()
	void RestoreBalance();

	UFUNCTION()
	void ChangeMovementType(bool RemoveTargetLock);

	UFUNCTION()
	void Teste();

	UFUNCTION()
	void GetUpTemp();

	UFUNCTION()
	void GetUpTemp2();

	UFUNCTION()
	void GetUpTemp3();

	UFUNCTION()
	void OpenSkillWhell();

	UFUNCTION()
	void CloseSkillWhell();

	UFUNCTION()
	void UpdateSkillWhell();

	UFUNCTION()
	void SpecialAtack();

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void AtualizarVidaPlayer();

	UFUNCTION()
	void OnParryMoment(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTakeHitEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnEnterExecutionMode(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEXitExecutionMode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnEXitWapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAtack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};
