#include "ActorComponents/Ability/UltimateAbilityComponent.h"

#include "Utilities/CLog.h"


UUltimateAbilityComponent::UUltimateAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UltimateGaugePercentage = (UltimateGauge / MaxUltimateGauge) * 100.f;
}

void UUltimateAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	StartAutoAddUltimateGauge();
}

void UUltimateAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUltimateAbilityComponent::UseAbility()
{
	if(bUltimateAvailable)
	{
		StartAbility();
		bUltimateAvailable = false;
	}
}

void UUltimateAbilityComponent::StartAbility()
{
	Super::StartAbility();
	ResetGauge();
}

void UUltimateAbilityComponent::ResetGauge()
{
	UltimateGauge = 0.f;
	UltimateGaugeChanged(0.f);
	Super::FinishAbility();
}

void UUltimateAbilityComponent::StartAutoAddUltimateGauge()
{
	if(GetOwner())
	{
		GetOwner()->GetWorldTimerManager().SetTimer(AutoAddUltimateGaugeTimerHandle, this, &UUltimateAbilityComponent::AutoAddGauge, 1.f, true, 1.f);
	}
}

void UUltimateAbilityComponent::StopAutoAddUltimateGauge()
{
	if(GetOwner())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(AutoAddUltimateGaugeTimerHandle);
	}
}

void UUltimateAbilityComponent::AutoAddGauge()
{
	AddUltimateGauge(AutoAddUltimateGaugeAmount);
}

void UUltimateAbilityComponent::UltimateGaugeChanged(const float InUltimateGaugePercentage) const
{
	if(OnUltimateGaugeChanged.IsBound())
	{
		OnUltimateGaugeChanged.Broadcast(InUltimateGaugePercentage);
	}
}

void UUltimateAbilityComponent::AddUltimateGauge(float InAmount)
{
	if(IsAbilityState(AbilityState, EAbilityState::EAS_Active)) return;
	if(bUltimateAvailable) return;
	
	if(UltimateGauge + InAmount >= MaxUltimateGauge)
	{
		UltimateGauge = MaxUltimateGauge;
		bUltimateAvailable = true;
	}
	else
	{
		UltimateGauge += InAmount;
	}

	UltimateGaugePercentage = UltimateGauge / MaxUltimateGauge;

	UltimateGaugeChanged(UltimateGaugePercentage);
}


