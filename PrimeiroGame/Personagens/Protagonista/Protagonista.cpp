// Fill out your copyright notice in the Description page of Project Settings.


#include "Protagonista.h"

#include "DrawDebugHelpers.h"
#include "ProtagonistaAninInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "PrimeiroGame/PrimeiroGame.h"
#include "PrimeiroGame/Personagens/InimigoPadrao/InimigoPadrao1.h"
#include "UI/RadialMenu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AProtagonista::AProtagonista()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmCamera->SetupAttachment(RootComponent);
	SpringArmCamera->TargetArmLength = 300.f;
	SpringArmCamera->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmCamera, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	CapsuleParry = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleParry"));
	CapsuleParry->SetupAttachment(GetMesh());

	BoxTarget = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxColision"));
	BoxTarget->SetupAttachment(Camera);

	CapsuleWeapon = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleWeapon"));
	CapsuleWeapon->SetupAttachment(GetMesh());

	BoxExecution = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxExecution"));
	BoxExecution->SetupAttachment(GetMesh());
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

// Called when the game starts or when spawned
void AProtagonista::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = CastChecked<APlayerController>(Controller);
	ProtagonistaAninInstance = Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance());

	CapsuleParry->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnParryMoment);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnTakeHit);
	BoxExecution->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnEnterExecutionMode);
	BoxExecution->OnComponentEndOverlap.AddDynamic(this, &AProtagonista::OnEXitExecutionMode);
	CapsuleWeapon->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnAtack);
}

// Called every frame
void AProtagonista::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bFocoEnemigo)
	{
		FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoFocado->GetTargetLocation());
		Rotacao.Pitch = 0.f;
		Rotacao.Roll = 0.f;
		GetController()->SetControlRotation(Rotacao);
	}
}

// Called to bind functionality to input
void AProtagonista::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoverFrente", this, &AProtagonista::MoveForward);
	PlayerInputComponent->BindAxis("MoverLado", this, &AProtagonista::MoveRight);
	PlayerInputComponent->BindAxis("OlharCimaBaixo", this, &AProtagonista::MoverMousePitch);
	PlayerInputComponent->BindAxis("OlharEsquerdaDireita", this, &AProtagonista::MoverMouseYaw);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProtagonista::PlayerJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Atacar", IE_Pressed, this, &AProtagonista::Atacar);
	
	PlayerInputComponent->BindAction("Focar", IE_Pressed, this, &AProtagonista::Focar);
	PlayerInputComponent->BindAction("TeleportMoviment", IE_Pressed, this, &AProtagonista::TeleportMoviment);
	PlayerInputComponent->BindAction("DesativarOverlap", IE_Pressed, this, &AProtagonista::DesativarOverlap);
	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &AProtagonista::ParryAtack);
	PlayerInputComponent->BindAction("Parry", IE_Released, this, &AProtagonista::StopParry);
	PlayerInputComponent->BindAction("Dash/Run", IE_Pressed, this, &AProtagonista::Dash);
	PlayerInputComponent->BindAction("Dash/Run", IE_Released, this, &AProtagonista::StopDash);
	PlayerInputComponent->BindAction("SkillWheel", IE_Pressed, this, &AProtagonista::OpenSkillWhell);
	PlayerInputComponent->BindAction("SkillWheel", IE_Released, this, &AProtagonista::CloseSkillWhell);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProtagonista::Teste);
}

void AProtagonista::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsMovementEnable)
		{
			// find out which way is forward
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
			// get forward vector
			AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
		} /*else if (bIsRotateEnable)
		{
			if (Value > 0.f)
			{
				SetActorRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
			} else
			{
				SetActorRotation(FRotator(0.f, GetControlRotation().Yaw+180.f, 0.f));
			}
		}*/
	}
}

void AProtagonista::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsMovementEnable)
		{
			// find out which way is right
			const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
			// get right vector 
			// add movement in that direction
			AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
		} /*else if (bIsRotateEnable)
		{
			if (Value > 0.f)
			{
				SetActorRotation(FRotator(0.f, GetControlRotation().Yaw+90.f, 0.f));
			} else
			{
				SetActorRotation(FRotator(0.f, GetControlRotation().Yaw-90.f, 0.f));
			}
		}*/
	
	}
}

void AProtagonista::MoverMousePitch(float Val)
{
	if (!bFocoEnemigo && !bIsInTeleportMoviment && Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		PlayerController->AddPitchInput(Val);
	}
}

void AProtagonista::MoverMouseYaw(float Val)
{
	if (!bFocoEnemigo && !bIsInTeleportMoviment && Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		PlayerController->AddYawInput(Val);
	}
}

void AProtagonista::Atacar()
{
	 if (bIsAtackEnable && !GetMovementComponent()->IsFalling())
	{
	 	ChangeAtackStatus(false);
	 	//ChangeMovementStatus(false);
		if (InimigoCampoVisao)
		{
			if (InimigoCampoVisao->GetStamina() == 100.f)
			{
				if (bIsPerfectParry)
				{
					ChangeRotator(false);
					FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoCampoVisao->GetTargetLocation());
					Rotacao.Pitch = 0.f;
					Rotacao.Roll = 0.f;
					SetActorRotation(Rotacao);
					FVector LocationTemp = GetActorLocation();
					PlayAnimMontage(Execution, 1, FName("ExecutionParry"));
					if (bFocoEnemigo)
					{
						Focar();
					}
					InimigoCampoVisao->TakeExecutionPerfectParry(GetActorForwardVector());
				}
				else
				{
					ChangeRotator(false);
					ExecutionEnemy();
					if (InimigoFocado)
					{
						Focar();
					}
				}
			} else if (AnguloInimigo > 140.f && !InimigoCampoVisao->IsSeePlayer())
			{
				ChangeRotator(false);
				ExecutionEnemy();
				if (InimigoFocado)
				{
					Focar();
				}
			} else	
			{
				if (bIsPerfectParry) IndexAtack = 3;
				StartSequenceAtack();
			}
		} else
		{
			StartSequenceAtack();
		}
	}
}

void AProtagonista::ExecutionEnemy()
{
	FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoCampoVisao->GetTargetLocation());
	Rotacao.Pitch = 0.f;
	Rotacao.Roll = 0.f;
	SetActorRotation(Rotacao);
	const bool Front = AnguloInimigo < 101.f && AnguloInimigo >= 0.f;
	FName ExecutionName;
	int Position = 102;
	if (Front) {
		Position = 102;
		ExecutionName = FName("ExecutionFront");
	}
	else
	{
		Position = 2.f * -1;
		ExecutionName = FName("ExecutionBack");
	}
	const FVector EnemyLocation = InimigoCampoVisao->GetActorLocation() + (InimigoCampoVisao->GetActorForwardVector() * Position);
	InimigoCampoVisao->OnTakeExecution(Front, this);
	SetActorLocation(EnemyLocation);
	PlayAnimMontage(Execution, 1, ExecutionName);
}

void AProtagonista::StartSequenceAtack()
{
	if (GetCharacterMovement()->MaxWalkSpeed > 600)
	{
		PlayAnimMontage(Atack, 1, FName("RunAtack"));
		return;
	}
	if (IndexAtack < 3) ChangeRotator(true);
		switch (IndexAtack)
		{
		case 0:
		{
			PlayAnimMontage(Atack, 1, FName("AtackSequence"));
			break;
		}
		case 1:
		{
			PlayAnimMontage(Atack, 1, FName("SecondAtack"));
			break;
		}
		case 2:
		{
			PlayAnimMontage(Atack, 1, FName("ThirtAtack"));
			break;
		}
		case 3:
		{
			PlayAnimMontage(Atack, 1, FName("AtackPerfectParry"));
			break;
		}
	}
}

void AProtagonista::OnAtack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInimigo* Enemy = Cast<AInimigo>(OtherActor))
	{
		Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode())->PlaySoundsWord(GetActorLocation(), 1);
		int Block = Enemy->TakeHit(this, AtackPower, AtackStaminaBreak, IndexAtack);
		if (Enemy->GetStamina() == 100.f)
		{
			ChangeAtackStatus(true);
		}
		else if (Block == 1)
		{
			ChangeAtackStatus(false);
			EnableDisabelOverlapWeapon(false);
			ChangeMovementStatus(false);
			ResetIndexAtackSequence();
			PlayAnimMontage(TakeBlock, 1, FName("PlayerTakeBlockBla"));
		}

	}
}

void AProtagonista::PlayerJump()
{
	if (!bIsAtackEnable) return;
	Jump();
}

void AProtagonista::Dash()
{
	if (!bIsAtackEnable) return;

	DisableMoviments(FName("Dash"));
	PlayAnimMontage(Moviment, 1, FName("Dash"));
	GetCharacterMovement()->MaxWalkSpeed = 1200;
	FRotator Temp1 = GetActorRotation();
	FRotator Temp2 = UKismetMathLibrary::Conv_VectorToRotator(GetLastMovementInputVector());
	UE_LOG(LogTemp, Warning, TEXT("Yaw1: %f, Yaw2: %f, 1-2: %f, 2-1: %f"), Temp1.Yaw, Temp2.Yaw, Temp1.Yaw - Temp2.Yaw, Temp2.Yaw - Temp1.Yaw);
	SetActorRotation(Temp2);
}

void AProtagonista::StopDash()
{
	TArray<FInputActionKeyMapping> Teclas;
	UInputSettings::GetInputSettings()->GetActionMappingByName(FName("Dash/Run"), Teclas);
	Teclas[0].Key;
	if (!PlayerController->IsInputKeyDown(Teclas[0].Key))
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
		SetRunState(false);
		//todo remove lock when is to far
		ChangeMovementType(InimigoFocado != nullptr);
	}
}

void AProtagonista::DashNotifyEnd()
{
	ResetAllStatus();
	StopDash();
}

void AProtagonista::DisableMoviments(FName Text)
{
	if (Text == FName("All"))
	{
		ChangeMovementStatus(false);
		ChangeAtackStatus(false);
		ChangeRotator(false);
		EnableDisabelOverlapWeapon(false);
	}
	else if (Text == FName("Dash"))
	{
		ChangeMovementType(true);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		EnableDisableOverlapMesh(false);
		ChangeRotator(false);
		ChangeAtackStatus(false);
		SetRunState(true);
	}
	
}

void AProtagonista::ResetAllStatus()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	ChangeMovementStatus(true);
	ChangeAtackStatus(true);
	ChangeRotator(true);
	ResetIndexAtackSequence();
	ResetPerfectPArry();
	EnableDisableOverlapMesh(true);
}

void AProtagonista::SetRunState(bool Val)
{
	Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance())->SetRunState(Val);
}


void AProtagonista::VerifyEnemyLockIsDead()
{
	if (InimigoFocado && bFocoEnemigo && InimigoFocado->IsDead())
	{
		Focar();
		InimigoCampoVisao = nullptr;
	}
}

void AProtagonista::Focar()
{
	
	if (bFocoEnemigo)
	{
		//Camera->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
		bFocoEnemigo = false;
		InimigoFocado->ChangeVisibilityUI(false);
		InimigoFocado = nullptr;
		
		Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance())->ChangeFocar();
		ChangeMovementType(true);
		return;
	}
	InimigoFocado = nullptr;
	BoxTarget->SetGenerateOverlapEvents(true);
	BoxTarget->SetRelativeScale3D(FVector(19.f, 8.f, 8.f));
	BoxTarget->SetRelativeLocation(FVector(980.f, 0.f, 0.f));
	
	TArray<AActor*> Bla;
	BoxTarget->GetOverlappingActors(Bla, ACharacter::StaticClass());
	for (auto Ble : Bla)
	{
		InimigoFocado = Cast<AInimigo>(Ble);
	}
	if (InimigoFocado)
	{
		//Camera->SetRelativeLocation(FVector(70.f, 0.f, 40.f));
		Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance())->ChangeFocar();
		bFocoEnemigo = true;
		InimigoFocado->ChangeVisibilityUI(true);
		ChangeMovementType(false);
	}
	BoxTarget->SetGenerateOverlapEvents(false);
	BoxTarget->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	BoxTarget->SetRelativeLocation(FVector(60.f, 0.f, 0.f));
}

void AProtagonista::ChangeMovementType(bool RemoveTargetLock)
{
	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
	if (RemoveTargetLock)
	{
		bUseControllerRotationYaw = false;
		if (MovementComponent)
		{
			MovementComponent->bOrientRotationToMovement = true;
		}
	}
	else
	{
		bUseControllerRotationYaw = true;
		if (MovementComponent)
		{
			//deactivate this for not tilt/flic animation when move
			MovementComponent->bOrientRotationToMovement = false;
		}
	}
}

void AProtagonista::TeleportMoviment()
{
	if (!bIsAtackEnable) return;
	APrimeiroGame* GameWord = Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode());
	
	if (!GameWord->GetLocalMovimentacao().IsZero()) {
		FVector OwnerDirection = Camera->GetForwardVector();
		FVector ActorDirection = GameWord->GetLocalMovimentacao() - Camera->GetComponentLocation();
		// Normalize vectors
		OwnerDirection.Normalize();
		ActorDirection.Normalize();

		// Calculate angle
		float Angulo = FMath::RadiansToDegrees(acosf(FVector::DotProduct(OwnerDirection, ActorDirection)));
		if (Angulo < 50)
		{
			ChangeMovementStatus(false);
			ChangeRotator(false);
			ChangeAtackStatus(false);
			bIsInTeleportMoviment = true;
			FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameWord->GetLocalMovimentacao());
			Rotacao.Pitch = 0.f;
			Rotacao.Roll = 0.f;
			SetActorRotation(Rotacao);
			GetController()->SetControlRotation(Rotacao);
			PlayAnimMontage(Moviment, 1.f, FName("ThrowTeleport"));	
		}
	}
}

void AProtagonista::ArriveTeleportMoviment()
{
	APrimeiroGame* GameWord = Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode());
	PlayAnimMontage(Moviment, 1.f, FName("TeleportArrive"));
	SetActorLocation(GameWord->GetLocalMovimentacao());
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	PlayerController->AddPitchInput(-5.f);
}


void AProtagonista::DesativarOverlap()
{
	UInputSettings *Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	if (!Settings) { return; }

	TArray<FInputActionKeyMapping> AxisMapping;
	UInputSettings::GetInputSettings()->GetActionMappingByName("Jump", AxisMapping);
	for (auto Axis : AxisMapping)
	{
		Settings->RemoveActionMapping(FInputActionKeyMapping("Jump", Axis.Key));
	}
	Settings->AddActionMapping(FInputActionKeyMapping("Jump", FKey("E")));
	Settings->SaveKeyMappings();
}

void AProtagonista::EnableDisabelOverlapWeapon(bool Val)
{
	CapsuleWeapon->SetGenerateOverlapEvents(Val);
}

void AProtagonista::OnEnterDefensePosition()
{
	ProtagonistaAninInstance->SetIsInDefensePosition(bIsInDefensePosition);
}

void AProtagonista::EnableDisableOverlapMesh(bool Val)
{
	GetCapsuleComponent()->SetGenerateOverlapEvents(Val);
}

void AProtagonista::ParryAtack()
{
	if (!bIsMovementEnable && !GetMovementComponent()->IsFalling()) return;
	bIsMovementEnable = false;
	bIsRotateEnable = false;
	PlayAnimMontage(InicioParry, 1.f, FName("InicioParry"));
	if (bIsInParryTime && InimigoCampoVisao)
	{
		bIsInParryTime = false;
		InimigoCampoVisao->ParryAnimation(ParryStaminaBreak, GetActorLocation());
		bIsPerfectParry = true;
		if (InimigoCampoVisao->GetStamina() == 100.f)
		{
			FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoCampoVisao->GetTargetLocation());
			Rotacao.Pitch = 0.f;
			Rotacao.Roll = 0.f;
			SetActorRotation(Rotacao);
			InimigoCampoVisao->ChangeExecuteMode(1.f);
			PlayAnimMontage(Execution, 1.f, FName("WaitExecution"));
			//Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode())->PlaySoundsWord(GetActorLocation(), 0);
		}
		else
		{
			PlayAnimMontage(InicioParry, 1.f, FName("PerfectParry"));
			InimigoCampoVisao->LaunchCharacter(-InimigoCampoVisao->GetActorForwardVector() * 700.f, true, true);
		}
	}
	else
	{
		bIsInDefensePosition = true;
		bIsInParryAnim = true;
	}
}

void AProtagonista::ResetPerfectPArry()
{
	bIsPerfectParry = false;
}

void AProtagonista::StopParry()
{
	bIsInDefensePosition = false;
	OnEnterDefensePosition();
	//ResetAllStatus();
}

void AProtagonista::OnParryMoment(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsInParryTime = true;
}

void AProtagonista::OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInimigo* Inimigo = Cast<AInimigo>(OtherActor))
	{
		APrimeiroGame* GameMode = Cast<APrimeiroGame>(UGameplayStatics::GetGameMode(GetWorld()));
		if (bIsInDefensePosition && InimigoCampoVisao)
		{
			ControlBalance(20);
			return;
		}
		bIsInParryTime = false;
		ControlBalance(5);
		GameMode->AtualizarVidaPlayer(VidaAtual -= 10);
		if (EquilibrioAtual == 100)
		{
			PlayAnimMontage(InicioParry, 1.f, TEXT("BalanceOut"));
		}
		else
		{
			PlayAnimMontage(TakeHit, 1.f, TEXT("TakeHit"));

		}
	}
}

void AProtagonista::ControlBalance(int Val)
{
	DisableMoviments(FName("All"));
	EquilibrioAtual += Val;
	if (EquilibrioAtual >= 100)
	{
		EquilibrioAtual = 100;
		LaunchCharacter(GetActorForwardVector() * 200.f * -1, true, false);
		PlayAnimMontage(InicioParry, 1.f, TEXT("BalanceOut"));
	}
	APrimeiroGame* GameMode = Cast<APrimeiroGame>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->AtualizarEquilibrioPlayer(EquilibrioAtual);
	GetWorldTimerManager().SetTimer(TimerHandlerBalance, this, &AProtagonista::RestoreBalance, 0.2f, true, 3.f);
}

void AProtagonista::RestoreBalance()
{
	if (EquilibrioAtual <= 0) {
		EquilibrioAtual = 0;
		GetWorldTimerManager().PauseTimer(TimerHandlerBalance);
		return;
	}
	EquilibrioAtual -= 5;
	APrimeiroGame* GameMode = Cast<APrimeiroGame>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->AtualizarEquilibrioPlayer(EquilibrioAtual);
}

bool AProtagonista::ValidarEnemigoCampoVisao(AActor* Actor)
{
	return Actor == InimigoCampoVisao;
}

void AProtagonista::OnEnterExecutionMode(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInimigo* Inimigo = Cast<AInimigo>(OtherActor))
	{
		//Sempre em primeiro
		InimigoCampoVisao = Inimigo;
		if (Inimigo->GetStamina() == 100.f)
		{
			Inimigo->ChangeExecuteMode(1.f);
			return;
		}
		FVector OwnerDirection = OtherActor->GetActorForwardVector();
		FVector ActorDirection = GetActorLocation() - OtherActor->GetActorLocation();
		// Normalize vectors
		OwnerDirection.Normalize();
		ActorDirection.Normalize();

		// Calculate angle
		AnguloInimigo = FMath::RadiansToDegrees(acosf(FVector::DotProduct(OwnerDirection, ActorDirection)));
		if (AnguloInimigo > 140.f && !Inimigo->IsSeePlayer())
		{
			Inimigo->ChangeExecuteMode(1.f);
		}
	}
}

void AProtagonista::OnEXitExecutionMode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsPerfectParry) return;
	if (AInimigo* Inimigo = Cast<AInimigo>(OtherActor))
	{
		AnguloInimigo = -1.f;
		Inimigo->ChangeExecuteMode(0.f);
		InimigoCampoVisao = nullptr;
	}
}

bool AProtagonista::ApproachEnemyUpdate(float Value)
{
	if (FVector::Dist(InimigoCampoVisao->GetActorLocation(), GetActorLocation()) <= 70.f)
	{
		return false;
	}
	FVector Location = FMath::Lerp(GetActorLocation(), InimigoCampoVisao->GetActorLocation(), Value);
	SetActorLocation(Location);
	return true;
}

void AProtagonista::ApproachEnemyFinished()
{
	FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoCampoVisao->GetTargetLocation());
	Rotacao.Pitch = 0.f;
	Rotacao.Roll = 0.f;
	SetActorRotation(Rotacao);
	InimigoCampoVisao->OnTakeExecution(AnguloInimigo < 101.f && AnguloInimigo >= 0.f, this);
	PlayAnimMontage(Execution, 1, FName("ExecutionBack"));
}

bool AProtagonista::MoveCameraTeleportUpdate(float Value)
{
	Camera->SetRelativeLocation(FVector(Value, 0.f, 10.f));
	return true;
}

void AProtagonista::MoveCameraTeleportFinished()
{
	ArriveTeleportMoviment();
}

void AProtagonista::OpenSkillWhell()
{
	if (RadialWheel == nullptr)
	{
		RadialWheel = Cast<URadialMenu>(CreateWidget(GetWorld(), RadialWheelClass));
	}

	if (IsValid(RadialWheel))
	{
		RadialWheel->AddToViewport();
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetIgnoreLookInput(true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, RadialWheel, EMouseLockMode::DoNotLock, false);
		RadialWheel->CentralizeCursor(PlayerController);
		if (GetWorldTimerManager().TimerExists(TimerHandlerWheelSpecial) && GetWorldTimerManager().IsTimerPaused(TimerHandlerWheelSpecial))
		{
			GetWorldTimerManager().UnPauseTimer(TimerHandlerWheelSpecial);
			UE_LOG(LogTemp, Warning, TEXT("resume timer"));
		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandlerWheelSpecial, this, &AProtagonista::UpdateSkillWhell, 0.1f, true, 0.1f);
			UE_LOG(LogTemp, Warning, TEXT("Criou timer"));
		}
	}
}

void AProtagonista::CloseSkillWhell()
{
	GetWorldTimerManager().PauseTimer(TimerHandlerWheelSpecial);
	RadialWheel->RemoveFromParent();
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetIgnoreLookInput(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
}

void AProtagonista::UpdateSkillWhell()
{
	RadialWheel->PosicaoMouseAtual(PlayerController);
}

void AProtagonista::Teste()
{
	if (RadialWheel == nullptr)
	{
		RadialWheel = Cast<URadialMenu>(CreateWidget(GetWorld(), RadialWheelClass));
		if (IsValid(RadialWheel))
		{
			RadialWheel->AddToViewport();
			//todo colocar o cursor no meio da tela
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetIgnoreLookInput(true);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, RadialWheel, EMouseLockMode::DoNotLock, false);
			GetWorldTimerManager().SetTimer(TimerHandlerWheelSpecial, this, &AProtagonista::RestoreBalance, 0.2f, true, 3.f);
		}
	}
	else
	{
		
	}
}