#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/AbilityType.h"
#include "PlayerCancelWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class OVERWATCH_API UPlayerCancelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void AbilityStateChanged(uint8 InAbilityState);

	UFUNCTION()
	void SetKeyText(FName InKeyName) const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> CancelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> KeyText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true", BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CancelWidgetAnimation;
};
