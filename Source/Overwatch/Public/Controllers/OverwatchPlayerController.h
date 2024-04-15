#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverwatchPlayerController.generated.h"

//class UInGameUI;
class UUserWidget;

UCLASS()
class OVERWATCH_API AOverwatchPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<UUserWidget> InGameUIClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> InGameUI;
};
