#include "AnimNotify/Genji/AN_Genji_SecondaryFire.h"
#include "Characters/Player/Genji/Genji.h"
#include "ActorComponents/Ability/Genji/Genji_SecondaryFireComponent.h"

FString UAN_Genji_SecondaryFire::GetNotifyName_Implementation() const
{
	return FString(TEXT("SecondaryFire"));
}

void UAN_Genji_SecondaryFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AGenji* GenjiRef = Cast<AGenji>(MeshComp->GetOwner());

	if (GenjiRef && GenjiRef->GetGenji_SecondaryFireComponent())
	{
		GenjiRef->GetGenji_SecondaryFireComponent()->TripleShot();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_Genji_SecondaryFire Notify GenjiRef or SecondaryFireComponent is nullptr"));
	}
}
