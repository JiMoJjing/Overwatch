#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "ActorComponents/Ability/AbilityComponent.h"

#include "Utilities.h"

UAbilityManagementComponent::UAbilityManagementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityManagementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityManagementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAbilityManagementComponent::NotifyAbilityStart(EAbilityType InAbilityType)
{
	ActiveAbilityType = InAbilityType;
	AbilityStarted(ActiveAbilityType);
}

void UAbilityManagementComponent::NotifyAbilityFinish(EAbilityType InAbilityType)
{
	AbilityFinished(InAbilityType);
	if (ActiveAbilityType == InAbilityType)
	{
		ActiveAbilityType = EAbilityType::EAT_None;
	}
}

void UAbilityManagementComponent::AbilityStarted(EAbilityType InAbilityType)
{
	if (OnAbilityStarted.IsBound())
	{
		OnAbilityStarted.Broadcast(InAbilityType);
	}
}

void UAbilityManagementComponent::AbilityFinished(EAbilityType InAbilityType)
{
	if (OnAbilityFinished.IsBound())
	{
		OnAbilityFinished.Broadcast(InAbilityType);
	}
}

