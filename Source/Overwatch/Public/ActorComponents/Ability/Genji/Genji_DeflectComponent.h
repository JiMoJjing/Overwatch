#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Genji_DeflectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UGenji_DeflectComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()

public:
	UGenji_DeflectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
