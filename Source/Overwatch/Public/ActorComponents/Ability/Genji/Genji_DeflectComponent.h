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
	void PlayAbilityMontage();
	
protected:
	virtual void UseAbility() override;
	virtual void StartAbility() override;
	virtual void FinishAbility() override;
	virtual void OnOtherAbilityStarted(EAbilityType InAbilityType) override;
	virtual void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted) override;
	
	virtual void DurationTimerEnd() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADeflectCollider> DeflectColliderClass;

	UPROPERTY()
	TObjectPtr<ADeflectCollider> DeflectCollider;

	UPROPERTY()
	FTimerHandle DeflectTimerHandle;

	UPROPERTY()
	uint8 DeflectMontageSectionCount = 0;
};
