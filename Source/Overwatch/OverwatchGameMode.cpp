#include "OverwatchGameMode.h"

#include "EngineUtils.h"
#include "Characters/CharacterBase.h"
#include "Controllers/OverwatchPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AOverwatchGameMode::AOverwatchGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AOverwatchGameMode::Server_ReceiveCharacterDeath(AController* DeadCharacterController, AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot)
{
	BroadcastCharacterDeathToControllers(DeadCharacterController, EventInstigator, DamageCauser, bIsHeadShot);
}

void AOverwatchGameMode::BroadcastCharacterDeathToControllers(AController* DeadCharacterController,	AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const AOverwatchPlayerController* OverwatchPlayerController = Cast<AOverwatchPlayerController>(It->Get());
		if (OverwatchPlayerController)
		{
			OverwatchPlayerController->HandleCharacterDeath(DeadCharacterController, EventInstigator, DamageCauser, bIsHeadShot);
		}
		if (OverwatchPlayerController == EventInstigator)
		{
			OverwatchPlayerController->KillAssistSuccess();
		}
	}
}
