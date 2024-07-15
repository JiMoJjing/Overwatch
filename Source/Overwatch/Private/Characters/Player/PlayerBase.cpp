#include "Characters/Player/PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Kismet/GameplayStatics.h"

#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "ActorComponents/Ability/UltimateAbilityComponent.h"
#include "ActorComponents/Ability/AmmoComponent.h"

#include "Controllers/OverwatchPlayerController.h"
#include "Utilities.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->GravityScale = 0.75f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->MaxAcceleration = 2048.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 100.f, 50.f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Team1Capsule")));
	GetMesh()->SetCollisionProfileName(FName(TEXT("Team1Mesh")));

	AbilityManagementComponent = CreateDefaultSubobject<UAbilityManagementComponent>(TEXT("AbilityManagementComponent"));
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	MovementModeChangedDelegate.AddDynamic(this, &APlayerBase::MovementModeChanged);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &APlayerBase::MoveForward);

		EnhancedInputComponent->BindAction(MoveBackwardAction, ETriggerEvent::Triggered, this, &APlayerBase::MoveForward);

		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &APlayerBase::MoveRight);

		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &APlayerBase::MoveRight);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerBase::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerBase::Jump);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerBase::StopJumping);

		EnhancedInputComponent->BindAction(AbilityOneAction, ETriggerEvent::Started, this, &APlayerBase::AbilityOne);

		EnhancedInputComponent->BindAction(AbilityTwoAction, ETriggerEvent::Started, this, &APlayerBase::AbilityTwo);

		EnhancedInputComponent->BindAction(AbilityThreeAction, ETriggerEvent::Started, this, &APlayerBase::AbilityThree);

		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &APlayerBase::Interaction);

		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this, &APlayerBase::PrimaryFire);

		EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this, &APlayerBase::SecondaryFire);

		EnhancedInputComponent->BindAction(ReloadingAction, ETriggerEvent::Started, this, &APlayerBase::Reloading);

		EnhancedInputComponent->BindAction(QuickMeleeAction, ETriggerEvent::Started, this, &APlayerBase::QuickMelee);
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBase::MoveForward(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	AddMovementInput(ForwardVector, InputValue);
}

void APlayerBase::MoveRight(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);

	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(RightVector, InputValue);
}

void APlayerBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerBase::Jump()
{
	Super::Jump();
}

void APlayerBase::StopJumping()
{
	Super::StopJumping();
}

void APlayerBase::StopMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void APlayerBase::CharacterDeath()
{
	
}

void APlayerBase::NotifyCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot)
{
	if(AOverwatchPlayerController* OverwatchPlayerController = Cast<AOverwatchPlayerController>(GetController()))
	{
		CLog::Print(TEXT("Death!"));
		OverwatchPlayerController->ReceiveCharacterDeath(EventInstigator, DamageCauser, bIsHeadShot);
	}
}

UAbilityComponent* APlayerBase::GetAbilityComponent(EAbilityType InAbilityType) const
{
	switch (InAbilityType)
	{
	case EAbilityType::EAT_PrimaryFire:
		return PrimaryFireComponent;
	case EAbilityType::EAT_SecondaryFire:
		return SecondaryFireComponent;
	case EAbilityType::EAT_AbilityOne:
		return AbilityOneComponent;
	case EAbilityType::EAT_AbilityTwo:
		return AbilityTwoComponent;
	case EAbilityType::EAT_AbilityThree:
		return UltimateAbilityComponent;
	case EAbilityType::EAT_QuickMelee:
		return QuickMeleeComponent;
		default:
			return nullptr;
	}
}

void APlayerBase::Interaction()
{
	CLog::Print(TEXT("Interaction Pressed"));
	FVector SocketOffset = CameraBoom->SocketOffset;
	SocketOffset = FVector(SocketOffset.X, SocketOffset.Y * -1, SocketOffset.Z);
	CameraBoom->SocketOffset = SocketOffset;
}

void APlayerBase::AbilityOne()
{
	CLog::Print(TEXT("Ability1 Pressed"));	
}

void APlayerBase::AbilityTwo()
{
	CLog::Print(TEXT("Ability2 Pressed"));
}

void APlayerBase::AbilityThree()
{
	CLog::Print(TEXT("Ability3 Pressed"));
}

void APlayerBase::PrimaryFire()
{
	CLog::Print(TEXT("PrimaryFire Triggered"));
}

void APlayerBase::SecondaryFire()
{
	CLog::Print(TEXT("SecondaryFire Triggered"));
}

void APlayerBase::Reloading()
{
	CLog::Print(TEXT("Reloading Pressed"));
	
	if(AmmoComponent)
	{
		AmmoComponent->UseAbility();
	}
}

void APlayerBase::QuickMelee()
{
	CLog::Print(TEXT("QuickMelee Pressed"));
}

void APlayerBase::ApplyDamageSuccess_Implementation(float Damage, bool bIsHeadShot)
{
	if(UltimateAbilityComponent)
	{
		UltimateAbilityComponent->AddUltimateGauge(Damage);
	}
	ApplyDamageSuccessEvent(bIsHeadShot);
}

bool APlayerBase::TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation,const ECollisionChannel InCollisionChannel)
{
	// 뷰포트 크기 구하기
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 스크린에서 크로스헤어 위치 구하기
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 플레이어 컨트롤러 참조
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController == nullptr) 
	{
		CLog::Log(TEXT("PlayerBase TraceUnderCrosshair PlayerController nullptr"));
		return false;
	}

	// 2D 화면 공간 좌표를 3D 공간 지점과 방향으로 변환
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(PlayerController, CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector TraceStartLocation = CrosshairWorldPosition;
		const FVector TraceEndLocation = TraceStartLocation + CrosshairWorldDirection * TraceDistance;

		if (UWorld* world = GetWorld())
		{
			world->LineTraceSingleByChannel(OutHitResult, TraceStartLocation, TraceEndLocation, InCollisionChannel);
		}
		else
		{
			CLog::Log(TEXT("PlayerBase TraceUnderCrosshair world nullptr"));
		}

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation(), OutHitLocation, FLinearColor::Yellow, 5.f, 1.f);
			return true;
		}
		OutHitLocation = TraceEndLocation;
	}
	return false;
}

bool APlayerBase::GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, const ECollisionChannel InCollisionChannel)
{
	FVector HitLocation;
	FHitResult HitResult;

	bool bTrace = TraceUnderCrosshair(50000.f, HitResult, HitLocation, InCollisionChannel);

	if (bTrace)
	{
		FVector Direction = HitResult.Location - StartLocation;
		Direction.Normalize();

		OutDirection = Direction;
		
		return true;
	}
	else
	{
		FVector Direction = HitLocation - StartLocation;
		Direction.Normalize();

		OutDirection = Direction;

		return true;
	}
}

void APlayerBase::MovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPrevCustomMovementMode)
{
	
}

void APlayerBase::ApplyDamageSuccessEvent(bool bIsHeadShot) const
{
	if(OnApplyDamageSuccessEvent.IsBound())
	{
		OnApplyDamageSuccessEvent.Broadcast(bIsHeadShot);
	}
}