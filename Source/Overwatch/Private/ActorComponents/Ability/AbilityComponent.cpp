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

	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		AbilityManagementComponent = PlayerBase->GetAbilityManagementComponent();
		PlayerBase->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UAbilityComponent::OnMontageInterrupted);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityComponent BeginPlay PlayerBase nullptr"));
	}

	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->OnAbilityStarted.AddDynamic(this, &UAbilityComponent::OnOtherAbilityStarted);
		AbilityManagementComponent->OnAbilityFinished.AddDynamic(this, &UAbilityComponent::OnOtherAbilityFinished);
	}

	AddAbilityState(AbilityState, EAbilityState::EAS_Available);
	AbilityStateChanged();
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAbilityComponent::UseAbility()
{
	if(CanUseAbility())
	{
		StartAbility();
	}
}

bool UAbilityComponent::CanUseAbility()
{
	bool bAvailable = IsAbilityState(AbilityState, EAbilityState::EAS_Available);
	bAvailable &= IsNotAbilityState(AbilityState, EAbilityState::EAS_Active);
	bAvailable &= IsNotAbilityState(AbilityState, EAbilityState::EAS_Cooldown);
	bAvailable &= CanCancelAbility();

	return bAvailable;
}

bool UAbilityComponent::CanCancelAbility()
{
	EAbilityType NowAbilityType = AbilityManagementComponent->GetActiveAbilityType();

	return (NowAbilityType == EAbilityType::EAT_None) || (CancelableAbilityTypes & static_cast<uint8>(NowAbilityType));
}

void UAbilityComponent::StartAbility()
{
	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->NotifyAbilityStart(AbilityType);
	}
	AddAbilityState(AbilityState, EAbilityState::EAS_Active);
	AbilityStateChanged();
}

void UAbilityComponent::FinishAbility()
{
	if (AbilityManagementComponent)
	{
		AbilityManagementComponent->NotifyAbilityFinish(AbilityType);
	}
	SubAbilityState(AbilityState, EAbilityState::EAS_Active);
	AbilityStateChanged();
}

void UAbilityComponent::OnOtherAbilityStarted(EAbilityType InAbilityType)
{
	if (MakeUnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
	{
		SubAbilityState(AbilityState, EAbilityState::EAS_Available);
		AbilityStateChanged();
	}
}

void UAbilityComponent::OnOtherAbilityFinished(EAbilityType InAbilityType)
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

void UAbilityComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if(AbilityMontage == Montage && bInterrupted)
	{
		FinishAbility();
	}
}

void UAbilityComponent::AbilityWidgetInit()
{
	AbilityStateChanged();
}

