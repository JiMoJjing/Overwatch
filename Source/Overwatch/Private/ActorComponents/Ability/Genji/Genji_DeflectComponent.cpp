#include "ActorComponents/Ability/Genji/Genji_DeflectComponent.h"



UGenji_DeflectComponent::UGenji_DeflectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGenji_DeflectComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGenji_DeflectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

