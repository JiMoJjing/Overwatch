#include "ActorComponents/Status/HPComponent.h"

#include "Characters/CharacterBase.h"


UHPComponent::UHPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHPComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	HPChanged();
}

void UHPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UHPComponent::TakeDamage(float InDamageAmount)
{
	float DamageAmount = InDamageAmount;

	if(CurrentHP <= DamageAmount)
	{
		DamageAmount = CurrentHP;
		CurrentHP = 0.f;
		CharacterDeath();
	}
	else
	{
		CurrentHP -= DamageAmount;
	}

	HPChanged();

	return DamageAmount;
}

float UHPComponent::TakeHeal(float InHealAmount)
{
	float HealAmount = InHealAmount;

	if(CurrentHP + InHealAmount >= MaxHP)
	{
		InHealAmount = MaxHP - CurrentHP;
		CurrentHP = MaxHP;
	}

	HPChanged();

	return InHealAmount;
}

void UHPComponent::CharacterDeath() const
{
	Cast<ACharacterBase>(GetOwner())->CharacterDeath();
}

void UHPComponent::HPChanged() const
{
	if(OnHPChanged.IsBound())
	{
		OnHPChanged.Broadcast(CurrentHP, MaxHP);
	}
}

