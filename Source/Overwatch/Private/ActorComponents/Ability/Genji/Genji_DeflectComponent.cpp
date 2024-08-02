#include "ActorComponents/Ability/Genji/Genji_DeflectComponent.h"

#include "Characters/Player/PlayerBase.h"
#include "Colliders/Genji/DeflectCollider.h"
#include "GameFramework/Character.h"
#include "Utilities.h"


UGenji_DeflectComponent::UGenji_DeflectComponent()
{
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
				DeflectCollider->SetGenji_DeflectComponent(this);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] DeflectColliderClass is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BeginPlay"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] CharacterBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BeginPlay"));
	}
}

void UGenji_DeflectComponent::PlayAbilityMontage()
{
	if(ACharacter* CharacterBase = Cast<ACharacter>(GetOwner()))
	{
		FName SectionName;
		switch (DeflectMontageSectionCount)
		{
		case 0:
			SectionName = FName(TEXT("Section_01"));
			break;
		case 1:
			SectionName = FName(TEXT("Section_02"));
			break;
		case 2:
			SectionName = FName(TEXT("Section_03"));
			break;
		case 3:
			SectionName = FName(TEXT("Section_04"));
			break;
			
		}
		CharacterBase->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
		CharacterBase->GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, AbilityMontage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] CharacterBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("PlayDeflectMontage"));
	}
	DeflectMontageSectionCount++;
	DeflectMontageSectionCount %= 4;
}

void UGenji_DeflectComponent::UseAbility()
{
	if(IsAbilityState(AbilityState, EAbilityState::EAS_Active))
	{
		DurationTimerEnd();
		return;
	}
	
	Super::UseAbility();
}

void UGenji_DeflectComponent::StartAbility()
{
	Super::StartAbility();
	if(DeflectCollider)
	{
		DeflectCollider->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] DeflectCollider is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("StartAbility"));
	}
	DurationTimerStart();
}

void UGenji_DeflectComponent::FinishAbility()
{
	Super::FinishAbility();
	if(DeflectCollider)
	{
		DeflectCollider->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] DeflectCollider is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("FinishAbility"));
	}
	CooldownTimerStart();
}

void UGenji_DeflectComponent::OnOtherAbilityStarted(EAbilityType InAbilityType)
{
	Super::OnOtherAbilityStarted(InAbilityType);

	if(IsAbilityState(AbilityState, EAbilityState::EAS_Active))
	{
		if(MakeUnavailableAbilityTypes & static_cast<uint8>(InAbilityType))
		{
			DurationTimerEnd();
		}
	}
}

void UGenji_DeflectComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	
}

void UGenji_DeflectComponent::DurationTimerEnd()
{
	Super::DurationTimerEnd();
	
	FinishAbility();
	
	if(AbilityMontage)
	{
		if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
		{
			PlayerBase->GetMesh()->GetAnimInstance()->Montage_Stop(0.5f, AbilityMontage);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] AbilityMontage is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("DurationEnd"));
	}
}

