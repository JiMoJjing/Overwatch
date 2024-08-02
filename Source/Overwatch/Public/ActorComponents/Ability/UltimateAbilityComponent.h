#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "UltimateAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUltimateGaugeChanged, float, InUltimateGaugePercentage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UUltimateAbilityComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:
	UUltimateAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UseAbility() override;

	UFUNCTION()
	void AddUltimateGauge(float InAmount);

protected:
	virtual void StartAbility() override;
	virtual void FinishAbility() override;

	void ResetGauge();
	void AutoAddGauge();
	
	void StartAutoAddUltimateGauge();
	void StopAutoAddUltimateGauge();

	void UltimateGaugeChanged(const float InUltimateGaugePercentage) const;

	void SetUltimateAbilityActive(bool bActive);

public:
	UPROPERTY(BlueprintAssignable)
	FOnUltimateGaugeChanged OnUltimateGaugeChanged;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Ultimate", meta=(AllowPrivateAccess = "true"))
	float MaxUltimateGauge = 2000.f;
	
	UPROPERTY()
	float UltimateGauge = 0.f;

	UPROPERTY()
	bool bUltimateAvailable = false;
	
	UPROPERTY(EditAnywhere, Category = "Ability_Ultimate", meta=(AllowPrivateAccess = "true"))
	float AutoAddUltimateGaugeAmount = 5.f;

	UPROPERTY()
	FTimerHandle AutoAddUltimateGaugeTimerHandle;

	UPROPERTY()
	bool bUltimateAbilityActive = false;
};
