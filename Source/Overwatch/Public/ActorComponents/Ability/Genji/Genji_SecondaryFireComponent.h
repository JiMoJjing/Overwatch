#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/AbilityComponent.h"
#include "Genji_SecondaryFireComponent.generated.h"

class UAnimMontage;
class UProjectilePoolComponent;
class UAmmoComponent;

UCLASS()
class OVERWATCH_API UGenji_SecondaryFireComponent : public UAbilityComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void UseAbility() override;

	UFUNCTION()
		void TripleShot();

protected:
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

	virtual void CooldownEnd() override;

private:
	void SecondaryFire();
	//void DelayTimerStart();
	//void DelayTimerEnd();
	//bool IsDelayTimerActive();
	void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);



private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SecondaryFireMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "10.0"))
	//float DelayTime = 0.68f;

	//UPROPERTY()
	//FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "45.0"))
	float Angle = 5.f;

	UPROPERTY()
	TObjectPtr<UProjectilePoolComponent> ProjectilePoolComponent;

	UPROPERTY()
	TObjectPtr<UAmmoComponent> AmmoComponent;
};
