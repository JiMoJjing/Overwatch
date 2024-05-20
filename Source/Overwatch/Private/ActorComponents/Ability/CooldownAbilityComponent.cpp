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

void UCooldownAbilityComponent::CooldownStart()
{
	AddAbilityState(AbilityState,EAbilityState::EAS_Cooldown);
	AbilityStateChanged();
	if (bHasCooldownWidget)
	{
		NowCooldownTime = CooldownDuration;
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownAbilityComponent::CooldownTimerTick, 0.1f, true, 0.f);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &UCooldownAbilityComponent::CooldownEnd, CooldownDuration, false);
	}
}

void UCooldownAbilityComponent::CooldownTimerTick()
{
	NowCooldownTime -= 0.1f;
	
	CooldownTimeChanged(NowCooldownTime);
	
	if (FMath::IsNearlyZero(NowCooldownTime, 0.01f))
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

void UCooldownAbilityComponent::CooldownTimeChanged(const float InNowCooldownTime)
{
	if(OnCooldownTimeChanged.IsBound())
	{
		OnCooldownTimeChanged.Broadcast(InNowCooldownTime, CooldownDuration);
	}
}

