#include "ActorComponents/Ability/Genji/Genji_PrimaryFireComponent.h"
#include "Characters/Player/Genji/Genji.h"
#include "Characters/Player/PlayerBase.h"
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
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] Genji is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BeginPlay"));
	}
}

void UGenji_PrimaryFireComponent::StartAbility()
{
	Super::StartAbility();

	PlayAbilityMontage();
	CooldownTimerStart();
}

void UGenji_PrimaryFireComponent::CooldownTimerEnd()
{
	Super::CooldownTimerEnd();
	FinishAbility();
}

void UGenji_PrimaryFireComponent::PlayAbilityMontage()
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
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] PlayerBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("PlayAbilityMontage"));
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] PlayerBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("SingleShot"));
	}
}