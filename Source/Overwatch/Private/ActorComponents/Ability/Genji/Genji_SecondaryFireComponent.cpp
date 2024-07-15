#include "ActorComponents/Ability/Genji/Genji_SecondaryFireComponent.h"
#include "Characters/Player/Genji/Genji.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Ability/AmmoComponent.h"

#include "Utilities.h"
#include "ActorComponents/Ability/ProjectileAmmoComponent.h"

void UGenji_SecondaryFireComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AGenji* Genji = Cast<AGenji>(GetOwner()))
	{
		ProjectileAmmoComponent = Cast<UProjectileAmmoComponent>(Genji->GetProjectileAmmoComponent());
	}
	else
	{
		CLog::Log(TEXT("UGenji_SecondaryFireComponent BeginPlay PlayerBase nullptr"));
	}
}

void UGenji_SecondaryFireComponent::StartAbility()
{
	Super::StartAbility();

	SecondaryFire();
	CooldownStart();
}

void UGenji_SecondaryFireComponent::CooldownEnd()
{
	Super::CooldownEnd();

	FinishAbility();
}

void UGenji_SecondaryFireComponent::SecondaryFire()
{
	if (!(AbilityManagementComponent && ProjectileAmmoComponent)) return;
	if (!ProjectileAmmoComponent->CanFire()) return;
	if (!AbilityMontage) return;

	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
	else
	{
		CLog::Log(TEXT("UGenji_SecondaryFireComponent UseAbility SecondaryFireMontage or PlayerBase nullptr"));
	}
}

void UGenji_SecondaryFireComponent::TripleShot()
{
	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		FVector StartLocation = PlayerBase->GetMesh()->GetSocketLocation(FName(TEXT("ShurikenStart")));
		FVector Direction;
		if (PlayerBase->GetDirectionToCrosshair(StartLocation, Direction, ECollisionChannel::ECC_GameTraceChannel9))
		{
			const FVector LeftDirection = Direction.RotateAngleAxis(-TripleShotAngle, FVector::UpVector);
			const FVector RightDirection = Direction.RotateAngleAxis(TripleShotAngle, FVector::UpVector);

			ProjectileAmmoComponent->ActivateProjectile(StartLocation, Direction);
			ProjectileAmmoComponent->ActivateProjectile(StartLocation, LeftDirection);
			ProjectileAmmoComponent->ActivateProjectile(StartLocation, RightDirection);

			ProjectileAmmoComponent->ConsumeAmmo(3);
		}
	}
}