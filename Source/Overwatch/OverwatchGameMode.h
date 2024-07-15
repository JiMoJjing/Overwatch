#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OverwatchGameMode.generated.h"

UCLASS(minimalapi)
class AOverwatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverwatchGameMode();

	void Server_ReceiveCharacterDeath(AController* DeadCharacterController, AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot);
	void BroadcastCharacterDeathToControllers(AController* DeadCharacterController, AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot);
};



