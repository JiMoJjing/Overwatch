#pragma once

#include "CoreMinimal.h"
#include "CooldownAbilityComponent.h"
#include "DurationAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDurationTimeChanged, float, RemainingDurationTime, float, DurationTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UDurationAbilityComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()

public:
	UDurationAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void DurationTimerStart();
	virtual void DurationTimerTick();
	virtual void DurationTimerEnd();

	void DurationTimeChanged(const float InRemainingDurationTime) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Duration", meta = (AllowPrivateAccess = "true"))
	float DurationTime = 0.f;

	float RemainingDurationTime = 0.f;
	
	UPROPERTY()
	FTimerHandle DurationTimerHandle;
	
	UPROPERTY(BlueprintAssignable)
	FOnDurationTimeChanged OnDurationTimeChanged;
};
