#include "ActorComponents/Ability/AbilityComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Ability/AbilityManagementComponent.h"

#include "Utilities.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerBase = Cast<APlayerBase>(GetOwner());

	if (PlayerBase)
	{
		AbilityManagementComponent = PlayerBase->GetAbilityManagementComponent();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityComponent BeginPlay PlayerBase nullptr"));
	}

	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->OnAbilityActivated.AddDynamic(this, &UAbilityComponent::OnAbilityActivated);
		AbilityManagementComponent->OnAbilityDeactivated.AddDynamic(this, &UAbilityComponent::OnAbilityDeactivated);
	}

	AddAbilityState(EAbilityState::EAS_Available);
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAbilityComponent::AddAbilityState(EAbilityState InAbilityState)
{
	AbilityState |= static_cast<uint8>(InAbilityState);
}

void UAbilityComponent::SubAbilityState(EAbilityState InAbilityState)
{
	AbilityState &= ~static_cast<uint8>(InAbilityState);
}

bool UAbilityComponent::CanActivateAbility()
{
	bool bAvailable = IsAbilityState(EAbilityState::EAS_Available) && IsNotAbilityState(EAbilityState::EAS_Active) && IsNotAbilityState(EAbilityState::EAS_Cooldown);
	bAvailable &= CanCancelAbility();

	return bAvailable;
}

bool UAbilityComponent::CanCancelAbility()
{
	bool bCanCancel = false;

	EAbilityType NowAbilityType = AbilityManagementComponent->GetAbilityType();

	return (NowAbilityType == EAbilityType::EAT_None) || (CancelableAbilityTypes & static_cast<uint8>(NowAbilityType));
}

void UAbilityComponent::ActivateAbility()
{
	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->ActivateAbility(AbilityType);
	}
	AddAbilityState(EAbilityState::EAS_Active);
}

void UAbilityComponent::DeactivateAbility()
{
	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->DeactivateAbility(AbilityType);
	}
	SubAbilityState(EAbilityState::EAS_Active);
}

void UAbilityComponent::OnAbilityActivated(EAbilityType InAbilityType)
{
	if (MakeUnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		SubAbilityState(EAbilityState::EAS_Available);
	}
}

void UAbilityComponent::OnAbilityDeactivated(EAbilityType InAbilityType)
{
	if (MakeUnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		AddAbilityState(EAbilityState::EAS_Available);
	}
}

void UAbilityComponent::CooldownStart()
{
	AddAbilityState(EAbilityState::EAS_Cooldown);

	if (bHasCooldownWidget)
	{
		NowCooldownTime = CooldownDuration;
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityComponent::CooldownTimerTick, 0.1f, true);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UAbilityComponent::CooldownEnd, CooldownDuration, false);
	}
}

void UAbilityComponent::CooldownTimerTick()
{
	NowCooldownTime -= 0.1f;
	CLog::Print(NowCooldownTime, 3, 0.1f, FColor::Yellow);

	if (FMath::IsNearlyZero(NowCooldownTime, 0.01f))
	{
		CooldownEnd();
	}

}

void UAbilityComponent::CooldownEnd()
{
	SubAbilityState(EAbilityState::EAS_Cooldown);

	if (GetOwner()->GetWorldTimerManager().IsTimerActive(CooldownTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
	}
}
