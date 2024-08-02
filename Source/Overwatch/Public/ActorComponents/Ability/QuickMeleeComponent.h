#pragma once

#include "CoreMinimal.h"
#include "CooldownAbilityComponent.h"
#include "QuickMeleeComponent.generated.h"


class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UQuickMeleeComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()

public:
	UQuickMeleeComponent();

protected:
	virtual void BeginPlay() override;
	virtual void StartAbility() override;

	virtual void CooldownTimerEnd() override;
private:
	void PlayAbilityMontage();
	void QuickMeleeSweep();
	void SpawnQuickMeleeHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_QuickMelee", meta = (AllowPrivateAccess = "true"))
	FVector QuickMeleeSweepBoxExtend = FVector(125.f, 125.f, 50.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_QuickMelee", meta = (AllowPrivateAccess = "true"))
	float QuickMeleeDamage = 40.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_QuickMelee", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> QuickMeleeHitEffect;
};
