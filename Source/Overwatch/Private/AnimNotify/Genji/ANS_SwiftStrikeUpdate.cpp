#include "AnimNotify/Genji/ANS_SwiftStrikeUpdate.h"
#include "Characters/Player/Genji/Genji.h"
#include "ActorComponents/Ability/Genji/SwiftStrikeComponent.h"

FString UANS_SwiftStrikeUpdate::GetNotifyName_Implementation() const
{
	return FString(TEXT("SwiftStrikeUpdate"));
}

void UANS_SwiftStrikeUpdate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	AGenji* genjiRef = Cast<AGenji>(MeshComp->GetOwner());

	if (genjiRef && genjiRef->GetSwiftStrikeComponent())
	{
		genjiRef->GetSwiftStrikeComponent()->SwiftStrikeUpdate(FrameDeltaTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ANS_SwiftStrikeUpdate NotifyTick genjiRef or SwiftStrikeComponent is nullptr"));
	}
}
