#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/TeamID.h"
#include "NPCWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class OVERWATCH_API UNPCWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnHPChanged(float InCurrentHP, float InMaxHP);

	void SetNPCNameTextBlock(const FText& InName) const;

	void SetColorByTeam(bool IsTeam) const;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UTextBlock> NPCNameTextBlock;
};
