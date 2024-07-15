#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverwatchPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillAssist);

class UUserWidget;

UCLASS()
class OVERWATCH_API AOverwatchPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	void ReceiveCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot);
	void HandleCharacterDeath(AController* DeadCharacterController,	AController* EventInstigator, AActor* DamageCauser, bool IsHeadShot) const;

	void KillAssistSuccess() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnKillAssist OnKillAssist;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<UUserWidget> InGameUIClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> InGameUI;
	
};
