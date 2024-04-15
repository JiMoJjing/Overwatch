#include "AnimNotify/Genji/AN_Genji_Reloading.h"
#include "Characters/Player/PlayerBase.h"
#include "ActorComponents/Ability/AmmoComponent.h"

FString UAN_Genji_Reloading::GetNotifyName_Implementation() const
{
	return FString(TEXT("Reloading"));
}

void UAN_Genji_Reloading::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayerBase* PlayerBase = Cast<APlayerBase>(MeshComp->GetOwner());

	if (PlayerBase)
	{
		if (UAmmoComponent* AmmoComponent = PlayerBase->GetAmmoComponent())
		{
			AmmoComponent->Reload();
		}
	}
}
