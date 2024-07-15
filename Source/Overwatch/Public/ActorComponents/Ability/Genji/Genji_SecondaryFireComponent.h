#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Genji_SecondaryFireComponent.generated.h"

class UAnimMontage;
class UProjectileAmmoComponent;
class UAmmoComponent;

UCLASS()
class OVERWATCH_API UGenji_SecondaryFireComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	// Called From AnimNotify 
	UFUNCTION()
	void TripleShot();

protected:
	virtual void StartAbility() override;
	virtual void CooldownEnd() override;

private:
	void SecondaryFire();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Ability_Genji", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "45.0"))
	float TripleShotAngle = 5.f;

	UPROPERTY()
	TObjectPtr<UProjectileAmmoComponent> ProjectileAmmoComponent;
};
