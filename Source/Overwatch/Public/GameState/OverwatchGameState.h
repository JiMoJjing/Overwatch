#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OverwatchGameState.generated.h"


UCLASS()
class OVERWATCH_API AOverwatchGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void ReceiveCharacterDeath(AController* DeadCharacterController, AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot);
};
