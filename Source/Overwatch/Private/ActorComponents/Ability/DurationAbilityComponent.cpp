#include "ActorComponents/Ability/DurationAbilityComponent.h"


UDurationAbilityComponent::UDurationAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDurationAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDurationAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDurationAbilityComponent::DurationStart()
{
	if(OnDurationTimeChanged.IsBound())
	{
		RemainingDurationTime = DurationTime;
		GetOwner()->GetWorldTimerManager().SetTimer(DurationTimerHandle, this, &UDurationAbilityComponent::DurationTick, 0.1f, true, 0.f);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().SetTimer(DurationTimerHandle, this, &UDurationAbilityComponent::DurationEnd, DurationTime, false);
	}
}

void UDurationAbilityComponent::DurationTick()
{
	RemainingDurationTime -= 0.1f;
	DurationTimeChanged(RemainingDurationTime);
	if(FMath::IsNearlyZero(RemainingDurationTime, 0.01f))
	{
		DurationEnd();
	}
}

void UDurationAbilityComponent::DurationEnd()
{
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(DurationTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(DurationTimerHandle);
	}
}

void UDurationAbilityComponent::DurationTimeChanged(const float InRemainingDurationTime) const
{
	OnDurationTimeChanged.Broadcast(InRemainingDurationTime, DurationTime);
}

