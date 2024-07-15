#include "Controllers/OverwatchNPCAIController.h"

#include "GameState/OverwatchGameState.h"


AOverwatchNPCAIController::AOverwatchNPCAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOverwatchNPCAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOverwatchNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOverwatchNPCAIController::ReceiveCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot)
{
	if(UWorld* World = GetWorld())
	{
		if(AOverwatchGameState* OverwatchGameState = Cast<AOverwatchGameState>(World->GetGameState()))
		{
			OverwatchGameState->ReceiveCharacterDeath(this, EventInstigator, DamageCauser, bIsHeadShot);
		}
	}
}

