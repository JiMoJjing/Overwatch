#include "Widgets/Player/PlayerCancelWidget.h"

#include "ActorComponents/Ability/AbilityComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/TextBlock.h"
#include "Enums/AbilityState.h"

void UPlayerCancelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	if(AbilityType != EAbilityType::EAT_None || AbilityType != EAbilityType::EAT_Max)
	{
		if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwningPlayerPawn()))
		{
			if(UAbilityComponent* AbilityComponent = PlayerBase->GetAbilityComponent(AbilityType))
			{
				AbilityComponent->OnAbilityStateChanged.AddDynamic(this, &UPlayerCancelWidget::AbilityStateChanged);
				SetKeyText(AbilityComponent->GetInputKeyName());
			}
		}
	}
}

void UPlayerCancelWidget::AbilityStateChanged(uint8 InAbilityState)
{
	if(IsAbilityState(InAbilityState, EAbilityState::EAS_Active))
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(CancelWidgetAnimation);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerCancelWidget::SetKeyText(FName InKeyName) const
{
	KeyText->SetText(FText::FromName(InKeyName));
}
