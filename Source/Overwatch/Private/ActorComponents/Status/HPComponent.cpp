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

float UHPComponent::TakeDamage(float InDamageAmount, bool& IsDeath)
{
	float DamageAmount = InDamageAmount;

	if(CurrentHP <= DamageAmount)
	{
		DamageAmount = CurrentHP;
		CurrentHP = 0.f;
		IsDeath = true;
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
	if(MaxHP == CurrentHP)
		return 0.f;
	
	float HealAmount = InHealAmount;

	if(CurrentHP + HealAmount >= MaxHP)
	{
		HealAmount = MaxHP - CurrentHP;
		CurrentHP = MaxHP;
	}

	HPChanged();

	return HealAmount;
}

void UHPComponent::CharacterDeath() const
{
	//Cast<ACharacterBase>(GetOwner())->CharacterDeath();
}

void UHPComponent::CharacterRevive()
{
	CurrentHP = MaxHP;
	HPChanged();
}

void UHPComponent::HPChanged() const
{
	if(OnHPChanged.IsBound())
	{
		OnHPChanged.Broadcast(CurrentHP, MaxHP);
	}
}

