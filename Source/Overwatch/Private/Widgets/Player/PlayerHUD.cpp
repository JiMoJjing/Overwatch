#include "Widgets/Player/PlayerHUD.h"

#include "Characters/Player/PlayerBase.h"
#include "Components/ProgressBar.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwningPlayerPawn()))
	{
		PlayerBase->OnApplyDamageSuccessEvent.AddDynamic(this, &UPlayerHUD::CreateHitMarker);
	}
}

void UPlayerHUD::CreateHitMarker(bool bIsHeadshot)
{
	FVector2D ViewportCenterPosition = GetPaintSpaceGeometry().GetLocalSize() / 2.f;
	
	if(bIsHeadshot)
	{
		if(HitMarker_Headshot)
		{
			UUserWidget* HitMarker = CreateWidget(GetOwningPlayer(), HitMarker_Headshot);
			HitMarker->SetPositionInViewport(ViewportCenterPosition - HitMarkerPositionOffset, false);
			HitMarker->AddToViewport();
		}
	}
	else
	{
		if(HitMarker_Normal)
		{
			UUserWidget* HitMarker = CreateWidget(GetOwningPlayer(), HitMarker_Normal);
			HitMarker->SetPositionInViewport(ViewportCenterPosition - HitMarkerPositionOffset, false);
			HitMarker->AddToViewport();
		}
	}
}
