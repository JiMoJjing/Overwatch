#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Genji_PrimaryFireComponent.generated.h"

class UProjectileAmmoComponent;
class UAmmoComponent;
class UAnimMontage;

UCLASS()
class OVERWATCH_API UGenji_PrimaryFireComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Called From AnimNotify
	UFUNCTION()
	void SingleShot();

protected:
	virtual void StartAbility() override;
	virtual void CooldownEnd() override;

private:
	void PrimaryFire();

private:
	UPROPERTY()
	TObjectPtr<UProjectileAmmoComponent> ProjectileAmmoComponent;
};
