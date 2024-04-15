#include "Characters/Player/Genji/Genji.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "ActorComponents/Ability/Genji/Genji_PrimaryFireComponent.h"
#include "ActorComponents/Ability/Genji/Genji_SecondaryFireComponent.h"
#include "ActorComponents/Ability/Genji/SwiftStrikeComponent.h"
#include "ActorComponents/Pools/ProjectilePoolComponent.h"
#include "Colliders/SwiftStrikeCollider.h"

#include "Utilities.h"


AGenji::AGenji() : JumpCount(0)
{
	SwiftStrikeComponent = CreateDefaultSubobject<USwiftStrikeComponent>(TEXT("SwiftStrikeComponent"));
	ProjectilePoolComponent = CreateDefaultSubobject<UProjectilePoolComponent>(TEXT("ProjectilePoolComponent"));
	PrimaryFireComponent = CreateDefaultSubobject<UGenji_PrimaryFireComponent>(TEXT("PrimaryFireComponent"));
	SecondaryFireComponent = CreateDefaultSubobject<UGenji_SecondaryFireComponent>(TEXT("SecondaryFireComponent"));
}

void AGenji::BeginPlay()
{
	Super::BeginPlay();
}

void AGenji::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGenji::Jump()
{
	switch (JumpCount)
	{
	case 0:
	{
		Super::Jump();
	}
		break;
	case 1:
	{
		SecondJump();
		JumpCount++;
	}
		break;
	default:
		break;
	}
}

void AGenji::StopJumping()
{
	Super::StopJumping();
}

void AGenji::JumpCountReset()
{
	JumpCount = 0;
}

void AGenji::MovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPrevCustomMovementMode)
{
	EMovementMode NewMovementMode = InCharacter->GetCharacterMovement()->MovementMode;

	switch (NewMovementMode)
	{
	case MOVE_None:
		break;
	case MOVE_Walking:
	{
		if (InPrevMovementMode == EMovementMode::MOVE_Falling)
		{
			JumpCountReset();
		}
	}
		break;
	case MOVE_NavWalking:
		break;
	case MOVE_Falling:
	{
		if(JumpCount == 0)
		{
			JumpCount = 1;
		}
	}
		break;
	case MOVE_Swimming:
		break;
	case MOVE_Flying:
		break;
	case MOVE_Custom:
		break;
	case MOVE_MAX:
		break;
	default:
		break;
	}

}

UGenji_PrimaryFireComponent* AGenji::GetPrimaryFireComponent() const
{
	if (PrimaryFireComponent)
	{
		return Cast<UGenji_PrimaryFireComponent>(PrimaryFireComponent);
	}
	else
	{
		CLog::Log(TEXT("AGenji GetPrimaryFireComponent PrimaryFireComponent nullptr"));
	}
	return nullptr;
}

UGenji_SecondaryFireComponent* AGenji::GetSecondaryFireComponent() const
{
	if (SecondaryFireComponent)
	{
		return Cast<UGenji_SecondaryFireComponent>(SecondaryFireComponent);
	}
	else
	{
		CLog::Log(TEXT("AGenji GetSecondaryFireComponent SecondaryFireComponent nullptr"));
	}
	return nullptr;
}

USwiftStrikeComponent* AGenji::GetSwiftStrikeComponent() const
{
	if (SwiftStrikeComponent)
	{
		return Cast<USwiftStrikeComponent>(SwiftStrikeComponent);
	}
	else
	{
		CLog::Log(TEXT("AGenji GetSwiftStrikeComponent SwiftStrikeComponent nullptr"));
	}
	return nullptr;
}

void AGenji::AbilityOne()
{
	if (SwiftStrikeComponent)
	{
		SwiftStrikeComponent->UseAbility();
	}
	else
	{
		CLog::Log(TEXT("AGenji AbilityOne SwiftStrikeComponent nullptr"));
	}
}

void AGenji::AbilityTwo()
{
}

void AGenji::AbilityThree()
{
}

void AGenji::PrimaryFire()
{
	if (PrimaryFireComponent)
	{
		PrimaryFireComponent->UseAbility();
	}
	else
	{
		CLog::Log(TEXT("AGenji PrimaryFire PrimaryFireComponent nullptr"));
	}
}

void AGenji::SecondaryFire()
{
	if (SecondaryFireComponent)
	{
		SecondaryFireComponent->UseAbility();
	}
	else
	{
		CLog::Log(TEXT("AGenji SecondaryFire SecondaryFireComponent nullptr"));
	}
}

void AGenji::Reloading()
{
	Super::Reloading();
}

void AGenji::QuickMelee()
{
}

void AGenji::SecondJump()
{
	LaunchCharacter(FVector(0.f, 0.f, 500.f), false, true);
}
