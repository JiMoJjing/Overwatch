#include "Characters/PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

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

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerBase::Reload);

		EnhancedInputComponent->BindAction(QuickMeleeAction, ETriggerEvent::Started, this, &APlayerBase::QuickMelee);
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBase::MoveForward(const FInputActionValue& Value)
{
	float inputValue = Value.Get<float>();

	const FRotator controlRotation = GetControlRotation();
	const FRotator yawRotation = FRotator(0.f, controlRotation.Yaw, 0.f);

	const FVector forwardVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(forwardVector, inputValue);
}

void APlayerBase::MoveRight(const FInputActionValue& Value)
{
	float inputValue = Value.Get<float>();

	const FRotator controlRotation = GetControlRotation();
	const FRotator yawRotation = FRotator(0.f, controlRotation.Yaw, 0.f);

	const FVector rightVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(rightVector, inputValue);
}

void APlayerBase::Look(const FInputActionValue& Value)
{
	FVector2D lookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
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

void APlayerBase::Interaction()
{
	CLog::Print(TEXT("Interaction Pressed"));
	FVector socketOffset = CameraBoom->SocketOffset;
	socketOffset = FVector(socketOffset.X, socketOffset.Y * -1, socketOffset.Z);
	CameraBoom->SocketOffset = socketOffset;
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

void APlayerBase::Reload()
{
	CLog::Print(TEXT("Reload Pressed"));
}

void APlayerBase::QuickMelee()
{
	CLog::Print(TEXT("QuickMelee Pressed"));
}

void APlayerBase::StopMovement()
{
	CLog::Print(TEXT("Stop Movement"), -1,  2.f, FColor::Red);
	GetCharacterMovement()->StopMovementImmediately();
}
