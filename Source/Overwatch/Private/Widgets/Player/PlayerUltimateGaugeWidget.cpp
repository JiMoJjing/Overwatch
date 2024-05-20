#include "Widgets/Player/PlayerUltimateGaugeWidget.h"

#include "ActorComponents/Ability/UltimateAbilityComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstance.h"

void UPlayerUltimateGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMaterialInstance* MaterialInstace = Cast<UMaterialInstance>(UltimateGaugeProgressBar->Brush.GetResourceObject());
	MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MaterialInstace, nullptr);
	UltimateGaugeProgressBar->Brush.SetResourceObject(MaterialInstanceDynamic);

	if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwningPlayerPawn()))
	{
		if(PlayerBase->GetUltimateAbilityComponent())
		{
			PlayerBase->GetUltimateAbilityComponent()->OnUltimateGaugeChanged.AddDynamic(this, &UPlayerUltimateGaugeWidget::OnUltimateGaugeChanged);
			PlayerBase->GetUltimateAbilityComponent()->AddUltimateGauge(0.f);
		}
	}
	UltimateImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerUltimateGaugeWidget::OnUltimateGaugeChanged(float InUltimateGaugePercentage)
{
	if(InUltimateGaugePercentage >= 1.f)
	{
		UltimateImage->SetVisibility(ESlateVisibility::Visible);
		UltimateGaugeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UltimateImage->SetVisibility(ESlateVisibility::Collapsed);
		UltimateGaugeTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	
	uint8 Percentage = static_cast<uint8>(InUltimateGaugePercentage * 100.f);
	FText Text = FText::FromString(FString::FromInt(Percentage));
	UltimateGaugeTextBlock->SetText(Text);
	MaterialInstanceDynamic->SetScalarParameterValue(TEXT("Percent"), InUltimateGaugePercentage);
}
