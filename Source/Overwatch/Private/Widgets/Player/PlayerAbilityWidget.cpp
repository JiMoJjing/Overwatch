#include "Widgets/Player/PlayerAbilityWidget.h"

#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerAbilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(AbilityType != EAbilityType::EAT_None || AbilityType != EAbilityType::EAT_Max)
	{
		if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwningPlayerPawn()))
		{
			UAbilityComponent* AbilityComponent = PlayerBase->GetAbilityComponent(AbilityType);
			if(AbilityComponent)
			{
				AbilityComponent->OnAbilityStateChanged.AddDynamic(this, &UPlayerAbilityWidget::UPlayerAbilityWidget::AbilityStateChanged);
				
				if(UTexture* Texture = AbilityComponent->GetAbilityTexture())
				{
					AbilityImage->SetBrushResourceObject(Texture);
				}
				AbilityComponent->AbilityWidgetInit();
			}
			if(UCooldownAbilityComponent* CooldownAbilityComponent = Cast<UCooldownAbilityComponent>(AbilityComponent))
			{
				CooldownAbilityComponent->OnCooldownTimeChanged.AddDynamic(this, &UPlayerAbilityWidget::CooldownTimeChanged);
			}
		}
	}
}

void UPlayerAbilityWidget::AbilityStateChanged(uint8 InAbilityState)
{
	bool IsAvailable = IsAbilityState(InAbilityState, EAbilityState::EAS_Available);
	bool IsCooldown = IsAbilityState(InAbilityState, EAbilityState::EAS_Cooldown);
	bool IsActive = IsAbilityState(InAbilityState, EAbilityState::EAS_Active);
	
	if(IsAvailable)
	{
		if(!IsCooldown && !IsActive)
		{
			BackgroundImage->SetBrushTintColor(BackgroundImageColor_Available);
		}
		if(IsCooldown)
		{
			CooldownBar->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		BackgroundImage->SetBrushTintColor(BackgroundImageColor_Unavailable);
		CooldownBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(IsCooldown)
	{
		CooldownBar->SetVisibility(ESlateVisibility::Visible);
		CooldownTextBlock->SetVisibility(ESlateVisibility::Visible);
		BackgroundImage->SetBrushTintColor(BackgroundImageColor_Cooldown);
	}
	else
	{
		CooldownBar->SetVisibility(ESlateVisibility::Collapsed);
		CooldownTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(IsActive)
	{
		BackgroundImage->SetBrushTintColor(BackgroundImageColor_Active);
	}

}

void UPlayerAbilityWidget::CooldownTimeChanged(float InNowCooldownTime, float InCooldownDuration)
{
	CooldownBar->SetPercent(1.f - InNowCooldownTime / InCooldownDuration);
	CooldownTextBlock->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(InNowCooldownTime))));
}