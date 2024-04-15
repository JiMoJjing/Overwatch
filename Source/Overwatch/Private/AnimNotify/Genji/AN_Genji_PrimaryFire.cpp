#include "AnimNotify/Genji/AN_Genji_PrimaryFire.h"
#include "Characters/Player/Genji/Genji.h"
#include "ActorComponents/Ability/Genji/Genji_PrimaryFireComponent.h"

FString UAN_Genji_PrimaryFire::GetNotifyName_Implementation() const
{
	return FString(TEXT("PrimaryFire"));
}

void UAN_Genji_PrimaryFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AGenji* GenjiRef = Cast<AGenji>(MeshComp->GetOwner());

	if (GenjiRef && GenjiRef->GetPrimaryFireComponent())
	{
		GenjiRef->GetPrimaryFireComponent()->SingleShot();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Genji_PrimaryFire Notify GenjiRef or PrimaryFireComponent is nullptr"));
	}
}
