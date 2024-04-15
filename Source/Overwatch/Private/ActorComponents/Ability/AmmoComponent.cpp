#include "ActorComponents/Ability/AmmoComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "Utilities.h"

UAmmoComponent::UAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickInterval = 0.1f;
	MaxAmmo = 0;
	CurrentAmmo = 0;
}

void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerBase)
	{
		PlayerBase->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UAmmoComponent::OnMontageInterrupted);
	}
	CurrentAmmo = MaxAmmo;
}

void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString str = FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo);
	CLog::Print(str, 10, 0.01f, FColor::White);
}

void UAmmoComponent::UseAbility()
{
	if (IsAmmoFull()) return;

	if (AbilityManagementComponent && AbilityManagementComponent->IsAbilityActivate())
	{
		bBufferedInput = true;
		return;
	}

	if (CanActivateAbility())
	{
		ActivateAbility();
	}
}

void UAmmoComponent::ActivateAbility()
{
	Super::ActivateAbility();

	PlayReloadingMontage();
}

void UAmmoComponent::DeactivateAbility()
{
	Super::DeactivateAbility();
}

void UAmmoComponent::OnAbilityDeactivated(EAbilityType InAbilityType)
{
	Super::OnAbilityDeactivated(AbilityType);

	if (bBufferedInput)
	{
		ActivateAbility();
		bBufferedInput = false;
	}
}

void UAmmoComponent::PlayReloadingMontage()
{
	if (ReloadingMontage && PlayerBase)
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(ReloadingMontage);
	}
}

void UAmmoComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == ReloadingMontage && bInterrupted)
	{
		DeactivateAbility();
	}
}

void UAmmoComponent::ConsumeAmmo(int32 InAmount)
{
	CurrentAmmo - InAmount > 0 ? CurrentAmmo -= InAmount : CurrentAmmo = 0;
}

void UAmmoComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
	DeactivateAbility();
}

