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

	AddAbilityState(AbilityState, EAbilityState::EAS_Available);
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAbilityComponent::UseAbility()
{
	if(CanActivateAbility())
	{
		ActivateAbility();
	}
}

bool UAbilityComponent::CanActivateAbility()
{
	bool bAvailable = IsAbilityState(AbilityState, EAbilityState::EAS_Available) && IsNotAbilityState(AbilityState, EAbilityState::EAS_Active) && IsNotAbilityState(AbilityState, EAbilityState::EAS_Cooldown);
	bAvailable &= CanCancelAbility();

	return bAvailable;
}

bool UAbilityComponent::CanCancelAbility()
{
	EAbilityType NowAbilityType = AbilityManagementComponent->GetAbilityType();

	return (NowAbilityType == EAbilityType::EAT_None) || (CancelableAbilityTypes & static_cast<uint8>(NowAbilityType));
}

void UAbilityComponent::ActivateAbility()
{
	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->ActivateAbility(AbilityType);
	}
	AddAbilityState(AbilityState, EAbilityState::EAS_Active);
	AbilityStateChanged();
}

void UAbilityComponent::DeactivateAbility()
{
	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->DeactivateAbility(AbilityType);
	}
	SubAbilityState(AbilityState, EAbilityState::EAS_Active);
	AbilityStateChanged();
}

void UAbilityComponent::OnAbilityActivated(EAbilityType InAbilityType)
{
	if (MakeUnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		SubAbilityState(AbilityState, EAbilityState::EAS_Available);
		AbilityStateChanged();
	}
}

void UAbilityComponent::OnAbilityDeactivated(EAbilityType InAbilityType)
{
	if (MakeUnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		AddAbilityState(AbilityState, EAbilityState::EAS_Available);
		AbilityStateChanged();
	}
}

void UAbilityComponent::AbilityStateChanged() const
{
	if(OnAbilityStateChanged.IsBound())
	{
		OnAbilityStateChanged.Broadcast(AbilityState);
	}
}

void UAbilityComponent::AbilityWidgetInit()
{
	AbilityStateChanged();
}

