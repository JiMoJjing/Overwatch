// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class OVERWATCH_API UEnemyHPBar : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnHPChanged(float InCurrentHP, float InMaxHP);

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UProgressBar> HPBar;
};
