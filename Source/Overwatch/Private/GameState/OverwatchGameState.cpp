#include "GameState/OverwatchGameState.h"

#include "OverwatchGameMode.h"

void AOverwatchGameState::ReceiveCharacterDeath(AController* DeadCharacterController, AController* EventInstigator,	AActor* DamageCauser, bool bIsHeadShot)
{
	if(UWorld* World = GetWorld())
	{
		if(AOverwatchGameMode* OverwatchGameMode = Cast<AOverwatchGameMode>(World->GetAuthGameMode()))
		{
			OverwatchGameMode->Server_ReceiveCharacterDeath(DeadCharacterController, EventInstigator, DamageCauser, bIsHeadShot);
		}
	}
}
