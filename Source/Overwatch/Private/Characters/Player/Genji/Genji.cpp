#include "Characters/Player/Genji/Genji.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ActorComponents/Ability/Genji/Genji_PrimaryFireComponent.h"
#include "ActorComponents/Ability/Genji/Genji_SecondaryFireComponent.h"
#include "ActorComponents/Ability/Genji/Genji_SwiftStrikeComponent.h"
#include "ActorComponents/Ability/Genji/Genji_DeflectComponent.h"
#include "ActorComponents/Ability/Genji/Genji_DragonbladeComponent.h"
#include "ActorComponents/Ability/ProjectileAmmoComponent.h"

#include "Controllers/OverwatchPlayerController.h"
#include "Utilities.h"
#include "ActorComponents/Ability/QuickMeleeComponent.h"
#include "ActorComponents/Ability/Genji/Genji_DragonbladeSlashComponent.h"
#include "Weapons/WeaponBase.h"


AGenji::AGenji()
{
	PrimaryFireComponent = CreateDefaultSubobject<UGenji_PrimaryFireComponent>(TEXT("PrimaryFireComponent"));
	SecondaryFireComponent = CreateDefaultSubobject<UGenji_SecondaryFireComponent>(TEXT("SecondaryFireComponent"));
	AbilityOneComponent = CreateDefaultSubobject<UGenji_SwiftStrikeComponent>(TEXT("Genji_SwiftStrikeComponent"));
	AbilityTwoComponent = CreateDefaultSubobject<UGenji_DeflectComponent>(TEXT("Genji_DeflectComponent"));
	UltimateAbilityComponent = CreateDefaultSubobject<UGenji_DragonbladeComponent>(TEXT("Genji_DragonbladeComponent"));
	AmmoComponent = CreateDefaultSubobject<UProjectileAmmoComponent>(TEXT("ProjectileAmmoComponent"));
	DragonbladeSlashComponent = CreateDefaultSubobject<UGenji_DragonbladeSlashComponent>(TEXT("DragonbladeSlashComponent"));
	QuickMeleeComponent = CreateDefaultSubobject<UQuickMeleeComponent>(TEXT("QuickMeleeComponent"));
}

void AGenji::BeginPlay()
{
	Super::BeginPlay();

	if(AOverwatchPlayerController* OverwatchPlayerController = Cast<AOverwatchPlayerController>(GetController()))
	{
		if(UGenji_SwiftStrikeComponent* SwiftStrikeComponent = Cast<UGenji_SwiftStrikeComponent>(AbilityOneComponent))
		{
			OverwatchPlayerController->OnKillAssist.AddDynamic(SwiftStrikeComponent, &UGenji_SwiftStrikeComponent::CooldownReset);
		}
	}

	if(DragonbladeClass)
	{
		Weapon_Dragonblade = GetWorld()->SpawnActor<AWeaponBase>(DragonbladeClass);
		
		if(Weapon_Dragonblade)
		{
			AttachDragonbladeTo(FName(TEXT("Dragonblade_Pelvis")));
		}
	}
}

void AGenji::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGenji::SetDragonbladeActive(bool bActive)
{
	bDragonbladeActive = bActive;

	if(bActive && OnDragonbladeActive.IsBound())
	{
		OnDragonbladeActive.Broadcast();
		AmmoComponent->Reload();
		AttachDragonbladeTo(FName(TEXT("Dragonblade_Hand")));
	}
	else if(bActive == false)
	{
		AttachDragonbladeTo(FName(TEXT("Dragonblade_Pelvis")));
	}
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

UProjectileAmmoComponent* AGenji::GetProjectileAmmoComponent() const
{
	if(AmmoComponent)
	{
		return Cast<UProjectileAmmoComponent>(AmmoComponent);
	}
	return nullptr;
}

UGenji_PrimaryFireComponent* AGenji::GetGenji_PrimaryFireComponent() const
{
	if (PrimaryFireComponent)
	{
		return Cast<UGenji_PrimaryFireComponent>(PrimaryFireComponent);
	}
	return nullptr;
}

UGenji_SecondaryFireComponent* AGenji::GetGenji_SecondaryFireComponent() const
{
	if (SecondaryFireComponent)
	{
		return Cast<UGenji_SecondaryFireComponent>(SecondaryFireComponent);
	}
	return nullptr;
}

UGenji_SwiftStrikeComponent* AGenji::GetGenji_SwiftStrikeComponent() const
{
	if (AbilityOneComponent)
	{
		return Cast<UGenji_SwiftStrikeComponent>(AbilityOneComponent);
	}
	return nullptr;
}

UGenji_DeflectComponent* AGenji::GetGenji_DeflectComponent() const
{
	if(AbilityTwoComponent)
	{
		return Cast<UGenji_DeflectComponent>(AbilityTwoComponent);
	}
	return nullptr;
}

UGenji_DragonbladeComponent* AGenji::GetGenji_DragonbladeComponent() const
{
	if(UltimateAbilityComponent)
	{
		return Cast<UGenji_DragonbladeComponent>(UltimateAbilityComponent);
	}
	return nullptr;
}

void AGenji::AbilityOne()
{
	if (AbilityOneComponent)
	{
		AbilityOneComponent->UseAbility();
	}
}

void AGenji::AbilityTwo()
{
	if(AbilityTwoComponent)
	{
		AbilityTwoComponent->UseAbility();
	}
}

void AGenji::AbilityThree()
{
	if(UltimateAbilityComponent)
	{
		UltimateAbilityComponent->UseAbility();
	}
}

void AGenji::PrimaryFire()
{
	if(bDragonbladeActive)
	{
		if(DragonbladeSlashComponent)
		{
			DragonbladeSlashComponent->UseAbility();
		}
		return;
	}
	if(PrimaryFireComponent)
	{
		PrimaryFireComponent->UseAbility();
	}
}

void AGenji::SecondaryFire()
{
	if(bDragonbladeActive)
	{
		if(DragonbladeSlashComponent)
		{
			DragonbladeSlashComponent->UseAbility();
		}
		return;
	}
	if(SecondaryFireComponent)
	{
		SecondaryFireComponent->UseAbility();
	}
}

void AGenji::Reloading()
{
	if(AmmoComponent)
	{
		AmmoComponent->UseAbility();
	}
}

void AGenji::QuickMelee()
{
	if(bDragonbladeActive)
	{
		if(DragonbladeSlashComponent)
		{
			DragonbladeSlashComponent->UseAbility();
		}
		return;
	}
	if(QuickMeleeComponent)
	{
		QuickMeleeComponent->UseAbility();
	}
}

void AGenji::ApplyDamageSuccess_Implementation(float Damage, bool bIsHeadShot)
{
	Super::ApplyDamageSuccess_Implementation(Damage, bIsHeadShot);
}

void AGenji::SecondJump()
{
	LaunchCharacter(FVector(0.f, 0.f, 500.f), false, true);
}

void AGenji::AttachDragonbladeTo(FName SocketName)
{
	if(Weapon_Dragonblade)
	{
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepRelative, false);
		Weapon_Dragonblade->AttachToComponent(GetMesh(), AttachmentTransformRules, SocketName);
	}
}
