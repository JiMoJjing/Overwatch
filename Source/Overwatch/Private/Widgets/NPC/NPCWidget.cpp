#include "Widgets/NPC/NPCWidget.h"

#include "ActorComponents/Status/HPComponent.h"
#include "Characters/CharacterBase.h"
#include "Components/ProgressBar.h"

#include "Utilities.h"
#include "Components/TextBlock.h"

void UNPCWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UNPCWidget::OnHPChanged(float InCurrentHP, float InMaxHP)
{
	float Percentage = InCurrentHP / InMaxHP;
	HPBar->SetPercent(Percentage);
}

void UNPCWidget::SetNPCNameTextBlock(const FText& InName) const
{
	NPCNameTextBlock->SetText(InName);
}

void UNPCWidget::SetColorByTeam(bool IsTeam) const
{
	if(IsTeam)
	{
		HPBar->SetFillColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		NPCNameTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 1.f, 1.f)));
	}
	else
	{
		HPBar->SetFillColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		NPCNameTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f, 1.f)));
	}
}
