#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerAmmoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class OVERWATCH_API UPlayerAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	UFUNCTION()
	void AmmoChanged(int32 InCurrentAmmo);

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> AmmoImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> AmmoTextBlock;

	UPROPERTY()
	int32 MaxAmmo;
	
};
