#include "AnimNotify/Genji/AN_SwiftStrikeFinished.h"
#include "Characters/Player/Genji/Genji.h"
#include "ActorComponents/Ability/Genji/SwiftStrikeComponent.h"

FString UAN_SwiftStrikeFinished::GetNotifyName_Implementation() const
{
	return FString(TEXT("SwiftStrikeFinished"));
}

void UAN_SwiftStrikeFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AGenji* genjiRef = Cast<AGenji>(MeshComp->GetOwner());

	if (genjiRef && genjiRef->GetSwiftStrikeComponent())
	{
		genjiRef->GetSwiftStrikeComponent()->SwiftStrikeMontageFinished();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AN_SwiftStrikeFinished Notify genjiRef or SwiftStrikeComponent is nullptr"));
	}
}
