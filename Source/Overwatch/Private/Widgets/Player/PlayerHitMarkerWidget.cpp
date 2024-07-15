#include "Widgets/Player/PlayerHitMarkerWidget.h"

void UPlayerHitMarkerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FWidgetAnimationDynamicEvent HitMarkerAnimationEvent;

	HitMarkerAnimationEvent.Clear();
	HitMarkerAnimationEvent.BindUFunction(this, FName(TEXT("HitMarkerAnimationEnded")));
	BindToAnimationFinished(HitMarkerAnimation, HitMarkerAnimationEvent);
	
	PlayAnimation(HitMarkerAnimation, 0, 1);
}

void UPlayerHitMarkerWidget::HitMarkerAnimationEnded()
{
	RemoveFromParent();
}
