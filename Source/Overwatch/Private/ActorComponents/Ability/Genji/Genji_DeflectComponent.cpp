#include "ActorComponents/Ability/Genji/Genji_DeflectComponent.h"

#include "Colliders/Genji/DeflectCollider.h"
#include "GameFramework/Character.h"


UGenji_DeflectComponent::UGenji_DeflectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CooldownTime = 8.f;

}

void UGenji_DeflectComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ACharacter* CharacterBase = Cast<ACharacter>(GetOwner()))
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = CharacterBase;
		ActorSpawnParams.Instigator = CharacterBase;

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		if (DeflectColliderClass)
		{
			DeflectCollider = GetWorld()->SpawnActor<ADeflectCollider>(DeflectColliderClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (DeflectCollider)
			{
				FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, true);
				DeflectCollider->AttachToActor(CharacterBase, TransformRules);
			}
		}
	}
}

void UGenji_DeflectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGenji_DeflectComponent::UseAbility()
{
	if(IsAbilityState(AbilityState, EAbilityState::EAS_Active))
	{
		DurationEnd();
		return;
	}
	
	Super::UseAbility();
}

void UGenji_DeflectComponent::StartAbility()
{
	Super::StartAbility();
	DeflectCollider->Activate();
	DurationStart();
}

void UGenji_DeflectComponent::FinishAbility()
{
	Super::FinishAbility();
	DeflectCollider->Deactivate();
	CooldownStart();
}

void UGenji_DeflectComponent::DurationEnd()
{
	Super::DurationEnd();
	FinishAbility();
}
