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

void UCooldownAbilityComponent::CooldownTimerStart()
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
		CooldownCurrentTime = CooldownTime;
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownAbilityComponent::CooldownTimerTick, 0.1f, true, 0.f);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownAbilityComponent::CooldownTimerEnd, CooldownTime, false);
	}
}

void UCooldownAbilityComponent::CooldownTimerEnd()
{
	SubAbilityState(AbilityState, EAbilityState::EAS_Cooldown);
	AbilityStateChanged();
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(CooldownTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
	}
}

void UCooldownAbilityComponent::CooldownTimerTick()
{
	CooldownCurrentTime -= 0.1f;
	
	CooldownTimeChanged(CooldownCurrentTime);
	
	if (FMath::IsNearlyZero(CooldownCurrentTime, 0.01f))
	{
		CooldownTimerEnd();
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
	CooldownTimerEnd();
}

