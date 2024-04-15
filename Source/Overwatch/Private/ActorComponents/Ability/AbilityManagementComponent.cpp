#include "ActorComponents/Ability/AbilityManagementComponent.h"
#include "ActorComponents/Ability/AbilityComponent.h"

#include "Utilities.h"

UAbilityManagementComponent::UAbilityManagementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ActiveAbilityComponent = nullptr;
}

void UAbilityManagementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityManagementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAbilityManagementComponent::ActivateAbility(EAbilityType InAbilityType)
{
	ActiveAbilityType = InAbilityType;
	AbilityActivated(ActiveAbilityType);
}

void UAbilityManagementComponent::DeactivateAbility(EAbilityType InAbilityType)
{
	AbilityDeactivated(InAbilityType);
	if (ActiveAbilityType == InAbilityType)
	{
		ActiveAbilityType = EAbilityType::EAT_None;
	}
}

void UAbilityManagementComponent::AbilityActivated(EAbilityType InAbilityType)
{
	if (OnAbilityActivated.IsBound())
	{
		OnAbilityActivated.Broadcast(InAbilityType);
		/*FString str = StaticEnum<EAbilityType>()->GetValueAsString(InAbilityType);
		CLog::Print(str);*/
	}
}

void UAbilityManagementComponent::AbilityDeactivated(EAbilityType InAbilityType)
{
	if (OnAbilityDeactivated.IsBound())
	{
		OnAbilityDeactivated.Broadcast(InAbilityType);
	}
}

