#include "Widgets/Player/PlayerHPBarWidget.h"

#include "ActorComponents/Status/HPComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Utilities/CLog.h"

void UPlayerHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwningPlayerPawn()))
	{
		if(PlayerBase->GetHPComponent())
		{
			PlayerBase->GetHPComponent()->OnHPChanged.AddDynamic(this, &UPlayerHPBarWidget::OnHPChanged);
		}
	}
}

void UPlayerHPBarWidget::OnHPChanged(float InCurrentHP, float InMaxHP)
{
	float Percentage = InCurrentHP / InMaxHP;
	
	CurrentHP = static_cast<uint8>(InCurrentHP);
	MaxHP = static_cast<uint8>(InMaxHP);

	HPBar->SetPercent(Percentage);
	
	FText HPText = FText::FromString(FString::FromInt(CurrentHP) + " / " + FString::FromInt(MaxHP));
	
	HPTextBlock->SetText(HPText);
}
