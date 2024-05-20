#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Genji_PrimaryFireComponent.generated.h"

class UProjectilePoolComponent;
class UAmmoComponent;
class UAnimMontage;

UCLASS()
class OVERWATCH_API UGenji_PrimaryFireComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;
	virtual void UseAbility() override;
	
	UFUNCTION()
	void SingleShot();

protected:
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

	virtual void CooldownEnd() override;

private:
	void PrimaryFire();
	void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> PrimaryFireMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "10.0"))
	//float DelayTime = 0.88f;

	//UPROPERTY()
	//FTimerHandle DelayTimerHandle;

	UPROPERTY()
	TObjectPtr<UProjectilePoolComponent> ProjectilePoolComponent;

	UPROPERTY()
	TObjectPtr<UAmmoComponent> AmmoComponent;
};
