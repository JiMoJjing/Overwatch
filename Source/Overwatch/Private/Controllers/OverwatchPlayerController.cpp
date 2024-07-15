#include "Controllers/OverwatchPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Utilities.h"
#include "Characters/NPC/NPCBase.h"
#include "GameFramework/Character.h"
#include "GameState/OverwatchGameState.h"

void AOverwatchPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(InGameUIClass)
	{
		InGameUI = CreateWidget(this, InGameUIClass, FName(TEXT("PlayerHUD")));
		InGameUI->AddToViewport();
	}
}

void AOverwatchPlayerController::ReceiveCharacterDeath(AController* EventInstigator, AActor* DamageCauser,	bool bIsHeadShot)
{
	if(UWorld* World = GetWorld())
	{
		if(AOverwatchGameState* OverwatchGameState = Cast<AOverwatchGameState>(World->GetGameState()))
		{
			OverwatchGameState->ReceiveCharacterDeath(this, EventInstigator, DamageCauser, bIsHeadShot);
		}
	}
}

void AOverwatchPlayerController::HandleCharacterDeath(AController* DeadCharacterController,	AController* EventInstigator, AActor* DamageCauser, bool IsHeadShot) const
{
	FString NPCName = "Null";
	if(Cast<ANPCBase>(DeadCharacterController->GetCharacter()))
		NPCName = (Cast<ANPCBase>(DeadCharacterController->GetCharacter())->GetNPCName()).ToString();
	
	FString str = EventInstigator->GetCharacter()->GetName() + " Kill " + NPCName + "!!";
	CLog::Print(str, -1, 5.f, FColor::Red);
}

void AOverwatchPlayerController::KillAssistSuccess() const
{
	if(OnKillAssist.IsBound())
	{
		OnKillAssist.Broadcast();
	}
}
