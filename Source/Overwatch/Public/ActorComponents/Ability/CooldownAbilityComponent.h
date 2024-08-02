#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "CooldownAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownTimeChanged, float, RemainingCooldownTime, float, CooldownTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UCooldownAbilityComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UCooldownAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void AbilityWidgetInit() override;

	UFUNCTION()
	void CooldownReset();
	
protected:
	virtual void CooldownTimerStart();
	virtual void CooldownTimerEnd();
	// if 'OnCooldownTimeChanged.IsBound()' is true, this called every 0.1 second by SetTimer
	virtual void CooldownTimerTick();

private:
	void CooldownTimeChanged(const float InRemainingCooldownTime) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownTimeChanged OnCooldownTimeChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Cooldown", meta = (AllowPrivateAccess = "true"))
	float CooldownTime = 8.f;

	float CooldownCurrentTime = 0.f;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	bool bReset = false;
};
