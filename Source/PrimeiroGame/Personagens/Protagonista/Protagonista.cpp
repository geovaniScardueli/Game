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
#include "PrimeiroGame/Personagens/Enemy/InimigoPadrao/InimigoPadrao1.h"
#include "UI/RadialMenu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyra.h"
#include "PrimeiroGame/Personagens/Boss/Nyra/BossNyraClone.h"
#include "PrimeiroGame/Personagens/Damage/Damage.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"


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

	PhysicalAnimationComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical Animation"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
}

// Called when the game starts or when spawned
void AProtagonista::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = CastChecked<APlayerController>(Controller);
	ProtagonistaAninInstance = Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance());

	CapsuleParry->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnParryMoment);
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnTakeHit);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AProtagonista::OnTakeHitEnd);
	BoxExecution->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnEnterExecutionMode);
	BoxExecution->OnComponentEndOverlap.AddDynamic(this, &AProtagonista::OnEXitExecutionMode);
	CapsuleWeapon->OnComponentBeginOverlap.AddDynamic(this, &AProtagonista::OnAtack);
	CapsuleWeapon->OnComponentEndOverlap.AddDynamic(this, &AProtagonista::OnEXitWapon);
	

	GameMode = Cast<APrimeiroGame>(UGameplayStatics::GetGameMode(GetWorld()));
	if (PhysicalAnimationComponent) PhysicalAnimationComponent->SetSkeletalMeshComponent(GetMesh());
	EnableDisabelOverlapWeapon(false);

	VidaAtual = 20;
	AtualizarVidaPlayer();
}

// Called every frame
void AProtagonista::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bFocoEnemigo && StatePlayer != States::sGrab)
	{
		FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoFocado->GetTargetLocation());
		Rotacao.Pitch = 0.f;
		Rotacao.Roll = 0.f;
		GetController()->SetControlRotation(Rotacao);
		//FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(SpringArmCamera->GetComponentLocation(), InimigoFocado->GetTargetLocation());
		//Rotacao.Pitch = 0.f;
		//Rotacao.Roll = 0.f;
		//Camera->SetWorldRotation(Rotacao);
		//SpringArmCamera->SetWorldRotation(Rotacao);
		
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
	PlayerInputComponent->BindAction("SpecialAtack", IE_Pressed, this, &AProtagonista::SpecialAtack);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProtagonista::Teste);
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AProtagonista::UseItem);
	
}

void AProtagonista::MoveForward(float Value)
{
	if (Value != 0.0f && (StatePlayer == States::sIdleMov || StatePlayer == States::sItem || StatePlayer == States::sStartAttack))
	{
		// find out which way is forward
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// get forward vector
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
	}
}

void AProtagonista::MoveRight(float Value)
{
	if (Value != 0.0f && (StatePlayer == States::sIdleMov || StatePlayer == States::sItem || StatePlayer == States::sStartAttack))
	{
		// find out which way is right
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// get right vector 
		// add movement in that direction
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
	}
}

void AProtagonista::MoverMousePitch(float Val)
{
	if (!bFocoEnemigo && Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		PlayerController->AddPitchInput(Val);
	}
}

void AProtagonista::MoverMouseYaw(float Val)
{
	if (!bFocoEnemigo && Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		PlayerController->AddYawInput(Val);
	}
}

void AProtagonista::Atacar()
{
	if (StatePlayer == States::sIdleMov || StatePlayer == States::sAttackEnd)
	{
		StatePlayer = States::sStartAttack;
		if (GetMovementComponent()->IsFalling())
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			PlayAnimMontage(ArrayMontage[AnimMontages::EPAirAtack], 1, FName("StartAirAtack"));
			return;
		}
		 
		//ChangeMovementStatus(false);
		if (InimigoCampoVisao && InimigoCampoVisao->ActorHasTag("DefaultEnemy"))
		{
			if (InimigoCampoVisao->GetStamina() == 100.f)
			{
				if (StatePlayer == States::sParry)
				{
					FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoCampoVisao->GetTargetLocation());
					FRotator Rotacao4 = FRotator(0.f, 0.f, Rotacao.Pitch * -1);
					Rotacao.Pitch = 0.f;
					Rotacao.Roll = 0.f;
					SetActorRotation(Rotacao);
					FVector LocationTemp = GetActorLocation();
					PlayAnimMontage(ArrayMontage[AnimMontages::EPExecution], 1, FName("ExecutionParry"));
					if (bFocoEnemigo)
					{
						Focar();
					}
					Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance())->SetRotationAtack(Rotacao4);
					InimigoCampoVisao->TakeExecutionPerfectParry(GetActorForwardVector());
				}
				else
				{
					ExecutionEnemy();
					if (InimigoFocado)
					{
						Focar();
					}
				}
			}
			else if (AnguloInimigo > 140.f && !InimigoCampoVisao->IsSeePlayer())
			{
				ExecutionEnemy();
				if (InimigoFocado)
				{
					Focar();
				}
			}
			else
			{
				if (StatePlayer == States::sParry) IndexAtack = 3;
				StartSequenceAtack();
			}
		}
		else
		{
			StartSequenceAtack();
		}
	}
}

void AProtagonista::StartSequenceAtack()
{
	if (GetCharacterMovement()->MaxWalkSpeed > 600)
	{
		PlayAnimMontage(ArrayMontage[AnimMontages::EPAtack], 1, FName("RunAtack"));
		return;
	}
	PlayAnimMontage(ArrayMontage[AnimMontages::EPAtack], 1.15f, AtackSequences[IndexAtack]);
}

void AProtagonista::StartAttack()
{
	EnableDisabelOverlapWeapon(true);
	StatePlayer = States::sAttack;
	//if (bFocoEnemigo) ChangeMovementType(true);
}

void AProtagonista::StopAttack()
{
	EnableDisabelOverlapWeapon(false);
	ChangeIndexAtackSequence();
	//if (bFocoEnemigo) ChangeMovementType(false);
}

void AProtagonista::ChangeIndexAtackSequence()
{
	StatePlayer = States::sAttackEnd;
	IndexAtack++;
	if (IndexAtack > 3)
	{
		ResetIndexAtackSequence();
	}
}

void AProtagonista::ResetIndexAtackSequence()
{
	IndexAtack = 0;
}

void AProtagonista::ExecutionEnemy()
{
	FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InimigoCampoVisao->GetTargetLocation());
	Rotacao.Pitch = 0.f;
	Rotacao.Roll = 0.f;
	SetActorRotation(Rotacao);
	const bool Front = AnguloInimigo < 101.f && AnguloInimigo >= 0.f;
	FName ExecutionName;
	int32 Position = 102;
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
	PlayAnimMontage(ArrayMontage[AnimMontages::EPExecution], 1, ExecutionName);
}

void AProtagonista::OnAtack(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("overlap atack"));
	if (AInimigo* Enemy = Cast<AInimigo>(OtherActor))
	{
		int32 Block = Enemy->TakeHit(AtackPower, IndexAtack);
		if (Enemy->GetStamina() == 100.f)
		{
			//todo tenho q ver o vai aqui
			//ChangeAtackStatus(true);
		}
		else if (Block == 1)
		{
			EnableDisabelOverlapWeapon(false);
			LaunchCharacter((GetActorForwardVector() * -1200.f), false, false);
			ResetIndexAtackSequence();
			PlayAnimMontage(ArrayMontage[AnimMontages::EPParyAtack], 0.5, FName("PlayerTakeBlock"));
		}
	}
	return;



	/*
	int32 Block;
	if (ABossNyra* Boss = Cast<ABossNyra>(OtherActor))
	{
		Block = Boss->BossTakeHit(AtackPower, AtackStaminaBreak, IndexAtack);
		if (Block == 1)
		{
			ChangeAtackStatus(false);
			EnableDisabelOverlapWeapon(false);
			ResetIndexAtackSequence();
			PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1, FName("ParryAtack1"));
		}
	} 
	else if (AInimigo* Enemy = Cast<AInimigo>(OtherActor))
	{
		Block = Enemy->TakeHit(AtackPower);
		if (Enemy->GetStamina() == 100.f)
		{
		}
		else if (Block == 1)
		{
			ChangeAtackStatus(false);
			EnableDisabelOverlapWeapon(false);
			ResetIndexAtackSequence();
			PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1, FName("PlayerTakeBlock"));
		}
		else
		{
			GameMode->PlaySoundsWord(GetActorLocation(), 1);
		}

	}
	*/
}

void AProtagonista::OnEXitWapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (AInimigo* Inimigo = Cast<AInimigo>(OtherActor))
	//{
	//	Inimigo->EnableDisableOverBody(true);
	//}

}

void AProtagonista::PlayerJump()
{
	if (StatePlayer != States::sIdleMov) return;
	Jump();
}

void AProtagonista::Dash()
{
	if (StatePlayer != States::sIdleMov) return;

	StatePlayer = States::sDash;
	PlayAnimMontage(ArrayMontage[AnimMontages::EPMoviment], 1.25f, FName("Dash"));
	//GetCharacterMovement()->MaxWalkSpeed = 1200;
	//FRotator Temp1 = GetActorRotation();
	//FRotator Temp2 = UKismetMathLibrary::Conv_VectorToRotator(GetLastMovementInputVector());
	//SetActorRotation(Temp2);
}

void AProtagonista::StopDash()
{
	//TArray<FInputActionKeyMapping> Teclas;
	//UInputSettings::GetInputSettings()->GetActionMappingByName(FName("Dash/Run"), Teclas);
	//Teclas[0].Key;
	//if (!PlayerController->IsInputKeyDown(Teclas[0].Key))
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = 600;
	//	SetRunState(false);
	//	//todo remove lock when is to far
	//	ChangeMovementType(InimigoFocado != nullptr);
	//}
}

void AProtagonista::DashNotifyEnd()
{
	ResetStatus("All");
	StopDash();
}

void AProtagonista::ResetStatus(const FName Status)
{
	if ("All" == Status)
	{
		StopAttack();
		Cast<UProtagonistaAninInstance>(GetMesh()->GetAnimInstance())->SetRotationAtack(FRotator(0.f, 0.f, 0.f));
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		StatePlayer = States::sIdleMov;
		ResetIndexAtackSequence();
		EnableDisableOverlapMesh(true);
		bIsInDefensePosition = false;
		bIsInParryAnim = false;
		OnEnterDefensePosition();
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else if ("Parry" == Status)
	{
		//StopAttack();
		bIsInParryAnim = false;
		bIsInParryTime = false;
		OnEnterDefensePosition();
		StatePlayer = States::sDefense;
	}
	else if ("EndDef" == Status)
	{
		//StopAttack();
		TArray<FInputActionKeyMapping> Teclas;
		UInputSettings::GetInputSettings()->GetActionMappingByName(FName("Parry"), Teclas);
		Teclas[0].Key;
		if (!PlayerController->IsInputKeyDown(Teclas[0].Key))
		{
			StopParry();
		}
	}
	else if ("EndParry" == Status)
	{
		StatePlayer = States::sEndParry;
	}
	else if ("Block" == Status)
	{
		EnableDisableOverlapMesh(true);
	}
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
		Camera->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
		
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
		Camera->SetRelativeLocation(FVector(10.f, 0.f, 40.f));
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
	//FName bla = Val ? FName("true") : FName("false");
	//if (Val) {
	//	UE_LOG(LogTemp, Warning, TEXT("true overlap"));
	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("false overlap"));
	//}
	CapsuleWeapon->SetCollisionResponseToChannel(ECC_PhysicsBody, Val ? ECR_Overlap : ECR_Ignore);
	//CapsuleWeapon->SetGenerateOverlapEvents(Val);
}

void AProtagonista::OnEnterDefensePosition()
{
	ProtagonistaAninInstance->SetIsInDefensePosition(bIsInDefensePosition);
}

void AProtagonista::EnableDisableOverlapMesh(bool Val)
{
	//if (Val) {
	//	UE_LOG(LogTemp, Warning, TEXT("trueOver"));
	//} else UE_LOG(LogTemp, Warning, TEXT("falseOver"));

	GetCapsuleComponent()->SetGenerateOverlapEvents(Val);
}

void AProtagonista::ParryAtack()
{

	if (StatePlayer != States::sIdleMov && StatePlayer != States::sAttackEnd && StatePlayer != States::sEndParry && StatePlayer != States::sStartParry) return;

	StatePlayer = States::sStartParry;
	if (CapsuleParry->GetOverlapInfos().Num() > 0)
	{
		const AActor* ActorOverlap = CapsuleParry->GetOverlapInfos()[0].OverlapInfo.Component.Get()->GetOwner();

		if (ActorOverlap->ActorHasTag("Clone"))
		{
			StatePlayer = States::sParry;
			//const FString Temp2 = FString("asdasdsa").Append("asdasdas");
			FString Temp = FString("PerfectParry");
			Temp.AppendInt(UKismetMathLibrary::RandomIntegerInRange(1, 3));
			//UE_LOG(LogTemp, Warning, TEXT("clone: %s"), *Temp);
			PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1.f, FName(*Temp));
			LaunchCharacter(GetActorForwardVector() * 1200.f * -1, true, false);
			return;
		}
		AInimigo* EnemyParry = Cast<AInimigo>(CapsuleParry->GetOverlapInfos()[0].OverlapInfo.Component.Get()->GetOwner());
		if (!EnemyParry) return;

		bIsInParryTime = false;
		EnemyParry->ParryAnimation(AtackPower, GetActorLocation());
		StatePlayer = States::sParry;
		if (EnemyParry->GetStamina() == 100.f)
		{
			FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyParry->GetTargetLocation());
			Rotacao.Pitch = 0.f;
			Rotacao.Roll = 0.f;
			SetActorRotation(Rotacao);
			EnemyParry->ChangeExecuteMode(1.f);
			PlayAnimMontage(ArrayMontage[AnimMontages::EPExecution], 1.f, FName("WaitExecution"));
			//Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode())->PlaySoundsWord(GetActorLocation(), 0);
		}
		else
		{
			if (EnemyParry->IsUnstoppableAttack())
			{
				LaunchCharacter(GetActorForwardVector() * 1200.f * -1, true, false);
			}
			FString Temp = FString("PerfectParry");
			Temp.AppendInt(UKismetMathLibrary::RandomIntegerInRange(1, 3));
			//UE_LOG(LogTemp, Warning, TEXT("defesa type: %s"), *Temp);
			PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1.f, FName(*Temp));
			EnemyParry->LaunchCharacter(-EnemyParry->GetActorForwardVector() * 1200.f, true, true);
		}
	}
	else
	{
		PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1.0f, FName("InicioParry"));
		bIsInDefensePosition = true;
		bIsInParryAnim = true;
		OnEnterDefensePosition();
	}
}

void AProtagonista::StopParry()
{
	if (StatePlayer != States::sDefense) return;

	bIsInDefensePosition = false;
	OnEnterDefensePosition();
	ResetStatus("All");
}

void AProtagonista::OnParryMoment(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsInParryTime = true;
	if (StatePlayer == States::sStartParry)
	{
		ParryAtack();
	}
}

void AProtagonista::OnTakeHitEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FHitResult bla;
	OnTakeHit(OverlappedComponent, OtherActor, OtherComp,
		OtherBodyIndex, false, bla);
}

void AProtagonista::OnTakeHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("player dano: %f"), 1.5f);
	EnableDisableOverlapMesh(false);
	if (StatePlayer == States::sParry) return;

	if (AInimigo* Inimigo = Cast<AInimigo>(OtherActor))
	{
		
		bIsInParryTime = false;
		if (bIsInDefensePosition && InimigoCampoVisao)
		{
			PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1.f, FName("Block"));
			GameMode->PlaySoundsWord(GetActorLocation(), 2);
			ControlBalance(UDamage::CalcaletePosture(Inimigo->GetAtackPower(), DefensePosture));
			return;
		}
		if (Inimigo->GetAtackSequence() == 5) {
			StatePlayer = States::sGrab;
			FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Inimigo->GetActorLocation());
			Rotacao.Pitch = 0.f;
			Rotacao.Roll = 0.f;
			SetActorRotation(Rotacao);
			PlayAnimMontage(ArrayMontage[AnimMontages::EPGrab], 1.f, FName("Grab"));
			Inimigo->ConectAttack("CloneGrab");
			EnableDisableOverlapMesh(true);

			return;
		}
		else if (Inimigo->GetAtackSequence() == 6) {
			PlayAnimMontage(ArrayMontage[AnimMontages::EPGrab], 1.f, FName("GrabEnd"));
		}

		FVector OwnerDirection = GetActorForwardVector();// OtherActor->GetActorForwardVector();
		FVector ActorDirection = OtherActor->GetActorLocation() - GetActorLocation();
		// Normalize vectors
		OwnerDirection.Normalize();
		ActorDirection.Normalize();

		// Calculate angle
		const float angulo = FMath::RadiansToDegrees(acosf(FVector::DotProduct(OwnerDirection, ActorDirection)));
		//UE_LOG(LogTemp, Warning, TEXT("angulo %f"), angulo);

		StatePlayer = States::sHit;
		PlayAnimMontage(ArrayMontage[AnimMontages::EPHit], 1.25f, HitSequences[angulo > 130 ? 1 : 0]);
		ControlBalance(UDamage::CalcaletePosture(Inimigo->GetAtackPower(), DefensePosture));
		VidaAtual -= UDamage::CalcaleteHealth(Inimigo->GetAtackPower(), Defense);
		AtualizarVidaPlayer();
		
	}
}

void AProtagonista::LevouPorrada(const bool Front)
{
	StatePlayer = States::sLock;
	VidaAtual -= 5;
	AtualizarVidaPlayer();
	ControlBalance(5);
	if (Front)
		PlayAnimMontage(ArrayMontage[AnimMontages::EPHit], 1.f, TEXT("TakeHitFront"));
	else
		PlayAnimMontage(ArrayMontage[AnimMontages::EPHit], 1.f, TEXT("TakeHitBack"));
}

void AProtagonista::Grab(const FName Name)
{
	if (Name == "Boss1")
	{
		PlayAnimMontage(ArrayMontage[AnimMontages::EPGrab], 1.f, TEXT("GrabEnd"));
	}
}

void AProtagonista::LaunchPlayer(const FVector Direction)
{
	PhysicalAnimationComponent->ApplyPhysicalAnimationProfileBelow("DEF-spine", "Strong");
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->AddImpulse(Direction, FName("DEF-spine"), true);
	ProtagonistaAninInstance->SetRagDoll(true);
}

void AProtagonista::ControlBalance(const int32 Val)
{
	EquilibrioAtual += Val;
	if (EquilibrioAtual >= 100)
	{
		EquilibrioAtual = 100;
		LaunchCharacter(GetActorForwardVector() * 1200.f * -1, true, false);
		PlayAnimMontage(ArrayMontage[AnimMontages::EPDefense], 1.f, TEXT("BalanceOut"));
	}
	GameMode->AtualizarEquilibrioPlayer(EquilibrioAtual);
	GetWorldTimerManager().SetTimer(TimerHandlerBalance, this, &AProtagonista::RestoreBalance, 0.2f, true, 3.f);
}

void AProtagonista::RestoreBalance()
{
	EquilibrioAtual -= 5;
	if (EquilibrioAtual <= 0) {
		EquilibrioAtual = 0;
		GameMode->AtualizarEquilibrioPlayer(EquilibrioAtual);
		GetWorldTimerManager().PauseTimer(TimerHandlerBalance);
		return;
	}
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
	else if (ABossNyra* Boss = Cast<ABossNyra>(OtherActor))
	{
		//Sempre em primeiro
		InimigoCampoVisao = Boss;
	}
}

void AProtagonista::OnEXitExecutionMode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (StatePlayer == States::sParry) return;

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
	PlayAnimMontage(ArrayMontage[AnimMontages::EPExecution], 1, FName("ExecutionBack"));
}

void AProtagonista::MoveCameraTeleportFinished()
{
	ArriveTeleportMoviment();
}

bool AProtagonista::MoveCameraTeleportUpdate(float Value)
{
	Camera->SetRelativeLocation(FVector(Value, 0.f, 20.f));
	return true;
}

void AProtagonista::MoveCamera()
{
	MoveCameraTeleport();
	ResetStatus("All");
}

void AProtagonista::TeleportMoviment()
{
	if (StatePlayer != States::sIdleMov) return;

	if (!GameMode->GetLocalMovimentacao().IsZero()) {
		FVector OwnerDirection = Camera->GetForwardVector();
		FVector ActorDirection = GameMode->GetLocalMovimentacao() - Camera->GetComponentLocation();
		// Normalize vectors
		OwnerDirection.Normalize();
		ActorDirection.Normalize();

		// Calculate angle
		float Angulo = FMath::RadiansToDegrees(acosf(FVector::DotProduct(OwnerDirection, ActorDirection)));
		if (Angulo < 50)
		{
			StatePlayer = States::sTeleport;
			FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GameMode->GetLocalMovimentacao());
			Rotacao.Pitch = 0.f;
			Rotacao.Roll = 0.f;
			SetActorRotation(Rotacao);
			GetController()->SetControlRotation(Rotacao);
			PlayAnimMontage(ArrayMontage[AnimMontages::EPMoviment], 1.f, FName("ThrowTeleport"));
		}
	}
}

void AProtagonista::ArriveTeleportMoviment()
{
	GetMesh()->SetVisibility(false);
	PlayAnimMontage(ArrayMontage[AnimMontages::EPMoviment], 1.f, FName("TeleportArrive"));
	APrimeiroGame* GameWord = Cast<APrimeiroGame>(GetWorld()->GetAuthGameMode());
	SetActorLocation(GameWord->GetLocalMovimentacao());
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	PlayerController->AddPitchInput(-5.f);

	//SwordBlueprint = GetWorld()->SpawnActor<AActor>(SwordBlueprintClass, GetMesh()->GetSocketTransform(FName("DEF-hand_R")));
	SwordBlueprint = GetWorld()->SpawnActor<AActor>(SwordBlueprintClass, GetActorLocation(), GetActorRotation());
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
		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandlerWheelSpecial, this, &AProtagonista::UpdateSkillWhell, 0.1f, true, 0.1f);
		}
	}
}

void AProtagonista::CloseSkillWhell()
{
	GetWorldTimerManager().PauseTimer(TimerHandlerWheelSpecial);
	IndexSelectSpecialAtack = RadialWheel->GetSelectSpecialAtack();
	RadialWheel->RemoveFromParent();
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetIgnoreLookInput(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
}

void AProtagonista::UpdateSkillWhell()
{
	RadialWheel->PosicaoMouseAtual(PlayerController);
}

void AProtagonista::SpecialAtack()
{
	if (IndexSelectSpecialAtack == 0.f)
	{
		StatePlayer = States::sLock;
		PlayAnimMontage(ArrayMontage[AnimMontages::EPSpecialAtack], 1, FName("SpecialAtack1"));
	}
}

void AProtagonista::AirTeleporPosition(FHitResult Bateu)
{
	CameraEnd = Bateu.Location;
	if (CameraEnd.IsZero())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		return;
	}
	else if (InimigoFocado)
	{
		AirMoveCameraTeleportFinished(InimigoFocado->GetActorLocation() + (GetActorForwardVector() * 20.f * -1.f));
		return;
	}
	FRotator Rotacao = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraEnd);
	Rotacao.Pitch = 0.f;
	Rotacao.Roll = 0.f;
	GetController()->SetControlRotation(Rotacao);
	if (AInimigo* Enemy = Cast<AInimigo>(Bateu.GetActor()))
	{
		CameraEnd.Z = Enemy->GetActorLocation().Z;
	}
	else
	{
		CameraEnd.Z += 100.f;
	}
	AirMoveCameraTeleport(Camera->GetComponentTransform());
}

void AProtagonista::AirMoveCameraTeleportFinished(FVector Localizacao)
{
	if (Localizacao.IsZero())
	{
		Localizacao = CameraEnd;
	}
	SetActorLocation(Localizacao);
	PlayAnimMontage(ArrayMontage[AnimMontages::EPAirAtack], 1, FName("AirAtackEnd"));
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AProtagonista::Teste()
{
	// tecla R
	if (false && GetMesh()->IsSimulatingPhysics(FName("DEF-spine")))
	{
		FVector PosBone = GetMesh()->GetSocketLocation(FName("DEF-spine"));
		PosBone.Z += 93;

		double bla = UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::GetRightVector(GetMesh()->GetSocketRotation(FName("DEF-spine_002"))), FVector(0.f, 0.f, 1.f));

		bIsFaceDown = bla < 0;

		FVector RotatinMesh;
		if (bIsFaceDown)
		{
			RotatinMesh = GetMesh()->GetSocketLocation("DEF-spine_005") - GetMesh()->GetSocketLocation("DEF-spine");
		}
		else
		{
			RotatinMesh = GetMesh()->GetSocketLocation("DEF-spine") - GetMesh()->GetSocketLocation("DEF-spine_005");
		}

		GetCapsuleComponent()->SetWorldLocation(PosBone);
		
		GetCapsuleComponent()->SetWorldRotation(UKismetMathLibrary::MakeRotFromZX(FVector(0.f, 0.f, 0.1f), RotatinMesh));

		//precisa desses timers para n�o 'flicar' as anima��es
		GetWorldTimerManager().SetTimer(TimerHandlerGetUp, this, &AProtagonista::GetUpTemp, 0.1f, false, 0.1f);
		
		return;
	}
	//GetMesh()->SetSimulatePhysics(true);
	//ProtagonistaAninInstance->SetRagDoll(true);
	//return;
	TArray<AActor*> foundEnemies;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossNyra::StaticClass(), foundEnemies);
	for (auto Ble : foundEnemies)
	{
		Cast<ABossNyra>(Ble)->TesteMethod();
		return;
	}
}

void AProtagonista::GetUpTemp()
{
	ProtagonistaAninInstance->SavePoseSnapshot(FName("EndPose"));
	GetWorldTimerManager().SetTimer(TimerHandlerGetUp, this, &AProtagonista::GetUpTemp2, 0.1f, false, 0.2f);
}

void AProtagonista::GetUpTemp2()
{
	GetMesh()->SetSimulatePhysics(false);
	ProtagonistaAninInstance->SetFaceDown(bIsFaceDown);
	

	PlayAnimMontage(ArrayMontage[EStandUp], 1, bIsFaceDown ? "Up" : "Down");
	GetWorldTimerManager().SetTimer(TimerHandlerGetUp, this, &AProtagonista::GetUpTemp3, 0.1f, false, 0.1f);
}

void AProtagonista::GetUpTemp3()
{
	ProtagonistaAninInstance->SetRagDoll(false);
}


void AProtagonista::TesteComBlueprint(AActor* Boss)
{
	Cast<ABossNyra>(Boss)->TesteMethod();
}


void AProtagonista::UseItem()
{
	if (StatePlayer != States::sIdleMov) return;

	StatePlayer = States::sItem;
	GetCharacterMovement()->MaxWalkSpeed = 200;
	PlayAnimMontage(ArrayMontage[AnimMontages::EHeal], 1, FName("Heal"));
}

void AProtagonista::ApplyEfectItem(const FName Item)
{
	if (Item == "Heal")
	{
		VidaAtual += MaxHealth * 0.4f;
		AtualizarVidaPlayer();
	}
}

void AProtagonista::AtualizarVidaPlayer()
{
	if (VidaAtual < 0) VidaAtual = 0;
	else if (VidaAtual > MaxHealth) VidaAtual = MaxHealth;
	GameMode->AtualizarVidaPlayer(VidaAtual);
}



