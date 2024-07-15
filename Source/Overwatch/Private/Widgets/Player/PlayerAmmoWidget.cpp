#include "Widgets/Player/PlayerAmmoWidget.h"

#include "ActorComponents/Ability/AmmoComponent.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPlayerAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwningPlayerPawn()))
	{
		if(UAmmoComponent* AmmoComponent = PlayerBase->GetAmmoComponent())
		{
			MaxAmmo = AmmoComponent->GetMaxAmmo();
			AmmoComponent->OnAmmoChanged.AddDynamic(this, &UPlayerAmmoWidget::AmmoChanged);

			if(UTexture* Texture = AmmoComponent->GetAbilityTexture())
			{
				AmmoImage->SetBrushResourceObject(Texture);
				AmmoImage->SetDesiredSizeOverride(FVector2D(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight()));
			}
		}
	}
}

void UPlayerAmmoWidget::AmmoChanged(int32 InCurrentAmmo)
{
	FText AmmoText = FText::FromString(FString::FromInt(InCurrentAmmo) + " / " + FString::FromInt(MaxAmmo));
	AmmoTextBlock->SetText(AmmoText);
}
