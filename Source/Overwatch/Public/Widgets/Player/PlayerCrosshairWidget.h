#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCrosshairWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class OVERWATCH_API UPlayerCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CrosshairImage;
};
