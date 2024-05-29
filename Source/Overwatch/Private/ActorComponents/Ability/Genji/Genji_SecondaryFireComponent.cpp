#include "ActorComponents/Ability/Genji/Genji_SecondaryFireComponent.h"
#include "Characters/Player/Genji/Genji.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Pools/ProjectilePoolComponent.h"
#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "ActorComponents/Ability/AmmoComponent.h"

#include "Utilities.h"

void UGenji_SecondaryFireComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerBase)
	{
		ProjectilePoolComponent = Cast<AGenji>(PlayerBase)->GetProjectilePoolComponent();
		AmmoComponent = PlayerBase->GetAmmoComponent();
		//GenjiRef->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UGenji_SecondaryFireComponent::MontageInterrupted);
	}
	else
	{
		CLog::Log(TEXT("UGenji_SecondaryFireComponent BeginPlay PlayerBase nullptr"));
	}
}

void UGenji_SecondaryFireComponent::UseAbility()
{
	Super::UseAbility();
}

void UGenji_SecondaryFireComponent::ActivateAbility()
{
	Super::ActivateAbility();

	SecondaryFire();
	CooldownStart();
}

void UGenji_SecondaryFireComponent::DeactivateAbility()
{
	Super::DeactivateAbility();
}

void UGenji_SecondaryFireComponent::CooldownEnd()
{
	Super::CooldownEnd();

	DeactivateAbility();
}


void UGenji_SecondaryFireComponent::TripleShot()
{
	if (!(PlayerBase && ProjectilePoolComponent && AmmoComponent))
	{
		CLog::Log(TEXT("UGenji_SecondaryFireComponent TripleShot PlayerBase && ProjectilePoolComponent && AmmoComponent nullptr"));
		return;
	}

	if (AmmoComponent->CanFire())
	{
		FVector StartLocation = PlayerBase->GetMesh()->GetSocketLocation(FName(TEXT("ShurikenStart")));
		FVector Direction;
		if (PlayerBase->GetDirectionToCrosshair(StartLocation, Direction, ECollisionChannel::ECC_GameTraceChannel9))
		{
			const FVector LeftDirection = Direction.RotateAngleAxis(-Angle, FVector::UpVector);
			const FVector RightDirection = Direction.RotateAngleAxis(Angle, FVector::UpVector);

			ProjectilePoolComponent->ActivateProjectile(StartLocation, Direction);
			ProjectilePoolComponent->ActivateProjectile(StartLocation, LeftDirection);
			ProjectilePoolComponent->ActivateProjectile(StartLocation, RightDirection);

			AmmoComponent->ConsumeAmmo(3);
		}
	}
}

void UGenji_SecondaryFireComponent::SecondaryFire()
{
	if (!(PlayerBase && AbilityManagementComponent && AmmoComponent)) return;
	if (!AmmoComponent->CanFire()) return;

	if (SecondaryFireMontage && PlayerBase)
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(SecondaryFireMontage);
	}
	else
	{
		CLog::Log(TEXT("UGenji_SecondaryFireComponent UseAbility SecondaryFireMontage or PlayerBase nullptr"));
	}
}

//void UGenji_SecondaryFireComponent::DelayTimerStart()
//{
//	if (GetOwner())
//	{
//		GetOwner()->GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &UGenji_SecondaryFireComponent::DelayTimerEnd, DelayTime, false);
//	}
//	else
//	{
//		CLog::Log(TEXT("UGenji_SecondaryFireComponent DelayTimerStart GetOwner() nullptr"));
//	}
//}
//
//void UGenji_SecondaryFireComponent::DelayTimerEnd()
//{
//	DeactivateAbility();
//}
//
//bool UGenji_SecondaryFireComponent::IsDelayTimerActive()
//{
//	bool bActive = false;
//
//	if (GetOwner())
//	{
//		bActive = GetOwner()->GetWorldTimerManager().IsTimerActive(DelayTimerHandle);
//	}
//
//	return bActive;
//}

void UGenji_SecondaryFireComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == SecondaryFireMontage && bInterrupted)
	{
		DeactivateAbility();
	}
}
