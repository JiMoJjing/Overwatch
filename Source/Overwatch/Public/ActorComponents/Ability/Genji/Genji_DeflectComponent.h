#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/DurationAbilityComponent.h"
#include "Genji_DeflectComponent.generated.h"


class ADeflectCollider;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UGenji_DeflectComponent : public UDurationAbilityComponent
{
	GENERATED_BODY()

public:
	UGenji_DeflectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void UseAbility() override;
	virtual void StartAbility() override;
	virtual void FinishAbility() override;

	virtual void DurationEnd() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADeflectCollider> DeflectColliderClass;

	UPROPERTY()
	TObjectPtr<ADeflectCollider> DeflectCollider;

	UPROPERTY()
	FTimerHandle DeflectTimerHandle;
};
