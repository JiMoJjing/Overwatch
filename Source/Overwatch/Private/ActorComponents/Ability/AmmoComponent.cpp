#include "ActorComponents/Ability/AmmoComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "Utilities.h"

UAmmoComponent::UAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxAmmo = 0;
	CurrentAmmo = 0;

	bBufferedInput = false;
}

void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	AmmoChanged();
}

void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAmmoComponent::UseAbility()
{
	if (IsAmmoFull()) return;

	if(AbilityManagementComponent && AbilityManagementComponent->IsAbilityActive())
	{
		bBufferedInput = true;
		return;
	}

	Super::UseAbility();
}

void UAmmoComponent::StartAbility()
{
	Super::StartAbility();

	PlayReloadingMontage();
}

void UAmmoComponent::FinishAbility()
{
	Super::FinishAbility();
}

void UAmmoComponent::OnOtherAbilityFinished(EAbilityType InAbilityType)
{
	Super::OnOtherAbilityFinished(AbilityType);

	if (bBufferedInput)
	{
		StartAbility();
	}
}

void UAmmoComponent::PlayReloadingMontage()
{
	if(AbilityMontage == nullptr) return;
	
	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
	}
}

void UAmmoComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if(AbilityMontage == Montage && bInterrupted)
	{
		FinishAbility();
		
		if(CurrentAmmo != 0)
		{
			bBufferedInput = false;
		}
	}
}

void UAmmoComponent::AmmoChanged() const
{
	if(OnAmmoChanged.IsBound())
	{
		OnAmmoChanged.Broadcast(CurrentAmmo);
	}
}

void UAmmoComponent::ConsumeAmmo(int32 InAmount)
{
	CurrentAmmo - InAmount > 0 ? CurrentAmmo -= InAmount : CurrentAmmo = 0;
	
	AmmoChanged();
	
	if(CurrentAmmo == 0)
	{
		bBufferedInput = true;
	}
}

void UAmmoComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
	bBufferedInput = false;
	AmmoChanged();
	FinishAbility();
}

