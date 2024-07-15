#include "ActorComponents/Ability/CooldownAbilityComponent.h"

UCooldownAbilityComponent::UCooldownAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCooldownAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCooldownAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCooldownAbilityComponent::AbilityWidgetInit()
{
	Super::AbilityWidgetInit();
	CooldownTimeChanged(CooldownTime);
}

void UCooldownAbilityComponent::CooldownStart()
{
	if(bReset)
	{
		bReset = false;
		return;
	}
	AddAbilityState(AbilityState,EAbilityState::EAS_Cooldown);
	AbilityStateChanged();
	if (OnCooldownTimeChanged.IsBound())
	{
		RemainingCooldownTime = CooldownTime;
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownAbilityComponent::CooldownTick, 0.1f, true, 0.f);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownAbilityComponent::CooldownEnd, CooldownTime, false);
	}
}

void UCooldownAbilityComponent::CooldownTick()
{
	RemainingCooldownTime -= 0.1f;
	
	CooldownTimeChanged(RemainingCooldownTime);
	
	if (FMath::IsNearlyZero(RemainingCooldownTime, 0.01f))
	{
		CooldownEnd();
	}
}

void UCooldownAbilityComponent::CooldownEnd()
{
	SubAbilityState(AbilityState, EAbilityState::EAS_Cooldown);
	AbilityStateChanged();
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(CooldownTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
	}
}

void UCooldownAbilityComponent::CooldownTimeChanged(const float InRemainingCooldownTime) const
{
	OnCooldownTimeChanged.Broadcast(InRemainingCooldownTime, CooldownTime);
}

void UCooldownAbilityComponent::CooldownReset()
{
	if(IsAbilityState(AbilityState, EAbilityState::EAS_Active))
	{
		bReset = true;
		return;
	}
	CooldownEnd();
}

