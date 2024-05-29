#include "ActorComponents/Ability/Genji/Genji_PrimaryFireComponent.h"
#include "Characters/Player/Genji/Genji.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Pools/ProjectilePoolComponent.h"
#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "ActorComponents/Ability/AmmoComponent.h"
#include "Utilities.h"


void UGenji_PrimaryFireComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerBase)
	{
		ProjectilePoolComponent = Cast<AGenji>(PlayerBase)->GetProjectilePoolComponent();
		AmmoComponent = PlayerBase->GetAmmoComponent();
		//GenjiRef->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UGenji_PrimaryFireComponent::MontageInterrupted);
	}
	else
	{
		CLog::Log(TEXT("UGenji_PrimaryFire BeginPlay PlayerBase nullptr"));
	}
}

void UGenji_PrimaryFireComponent::UseAbility()
{
	Super::UseAbility();
}

void UGenji_PrimaryFireComponent::ActivateAbility()
{
	Super::ActivateAbility();

	PrimaryFire();
	CooldownStart();
}

void UGenji_PrimaryFireComponent::DeactivateAbility()
{
	Super::DeactivateAbility();
}

void UGenji_PrimaryFireComponent::CooldownEnd()
{
	Super::CooldownEnd();
	DeactivateAbility();
}

void UGenji_PrimaryFireComponent::PrimaryFire()
{
	if (!(PlayerBase && AbilityManagementComponent && AmmoComponent)) return;
	if (!AmmoComponent->CanFire()) return;

	if (PrimaryFireMontage)
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(PrimaryFireMontage);
	}
	else
	{
		CLog::Log(TEXT("UGenji_PrimaryFireComponent UseAbility PrimaryFireMontage or PlayerBase nullptr"));
	}
}

void UGenji_PrimaryFireComponent::SingleShot()
{
	if (!(PlayerBase && ProjectilePoolComponent && AmmoComponent))
	{
		CLog::Log(TEXT("UGenji_PrimaryFireComponent SingleShot PlayerBase && ProjectilePoolComponent && AmmoComponent nullptr"));
		return;
	}

	if (AmmoComponent->CanFire())
	{
		FVector StartLocation = PlayerBase->GetMesh()->GetSocketLocation(FName(TEXT("ShurikenStart")));
		FVector Direction;
		if (PlayerBase->GetDirectionToCrosshair(StartLocation, Direction, ECollisionChannel::ECC_GameTraceChannel9))
		{
			ProjectilePoolComponent->ActivateProjectile(StartLocation, Direction);
			AmmoComponent->ConsumeAmmo(1);
		}
	}
}

void UGenji_PrimaryFireComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == PrimaryFireMontage && bInterrupted)
	{
		DeactivateAbility();
	}
}