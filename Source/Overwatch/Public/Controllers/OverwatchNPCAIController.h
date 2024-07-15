#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OverwatchNPCAIController.generated.h"

UCLASS()
class OVERWATCH_API AOverwatchNPCAIController : public AAIController
{
	GENERATED_BODY()

public:
	AOverwatchNPCAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ReceiveCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot);
};
