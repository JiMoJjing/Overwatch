#include "ActorComponents/Ability/Genji/Genji_PrimaryFireComponent.h"
#include "Characters/Player/Genji/Genji.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Pools/ProjectilePoolComponent.h"
#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "ActorComponents/Ability/AmmoComponent.h"
#include "Utilities.h"
#include "ActorComponents/Ability/ProjectileAmmoComponent.h"


void UGenji_PrimaryFireComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AGenji* Genji = Cast<AGenji>(GetOwner()))
	{
		ProjectileAmmoComponent = Cast<UProjectileAmmoComponent>(Genji->GetProjectileAmmoComponent());
	}
	else
	{
		CLog::Log(TEXT("UGenji_PrimaryFire BeginPlay PlayerBase nullptr"));
	}
}

void UGenji_PrimaryFireComponent::StartAbility()
{
	Super::StartAbility();

	PrimaryFire();
	CooldownStart();
}

void UGenji_PrimaryFireComponent::CooldownEnd()
{
	Super::CooldownEnd();
	FinishAbility();
}

void UGenji_PrimaryFireComponent::PrimaryFire()
{
	if (AbilityManagementComponent == nullptr && ProjectileAmmoComponent == nullptr) return;
	if (AbilityMontage == nullptr) return;
	if (!ProjectileAmmoComponent->CanFire()) return;
	
	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
	else
	{
		CLog::Log(TEXT("UGenji_PrimaryFireComponent UseAbility PrimaryFireMontage or PlayerBase nullptr"));
	}
}

void UGenji_PrimaryFireComponent::SingleShot()
{
	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		FVector StartLocation = PlayerBase->GetMesh()->GetSocketLocation(FName(TEXT("ShurikenStart")));
		FVector Direction;
		if (PlayerBase->GetDirectionToCrosshair(StartLocation, Direction, ECollisionChannel::ECC_GameTraceChannel9))
		{
			ProjectileAmmoComponent->ActivateProjectile(StartLocation, Direction);
			ProjectileAmmoComponent->ConsumeAmmo(1);
		}
	}
}
