#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/AbilityState.h"
#include "Enums/AbilityType.h"
#include "PlayerAbilityWidget.generated.h"


class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class OVERWATCH_API UPlayerAbilityWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void AbilityStateChanged(uint8 InAbilityState);

	UFUNCTION()
	void CooldownTimeChanged(float InNowCooldownTime, float InCooldownDuration);

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UImage> AbilityImage;

	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> CooldownBar;
	
	UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> CooldownTextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor BackgroundImageColor_Available;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor BackgroundImageColor_Active;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor BackgroundImageColor_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FColor BackgroundImageColor_Unavailable;
};
