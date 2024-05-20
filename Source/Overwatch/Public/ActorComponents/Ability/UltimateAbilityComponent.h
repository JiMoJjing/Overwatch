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

	void StartAutoAddUltimateGauge();
	void StopAutoAddUltimateGauge();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetUltimateGaugePercentage() const { return UltimateGaugePercentage; }
	
	UFUNCTION()
	void AddUltimateGauge(float InAmount);
	
	UPROPERTY(BlueprintAssignable)
	FOnUltimateGaugeChanged OnUltimateGaugeChanged;

protected:
	virtual void ActivateAbility() override;

	void ResetGauge();
	void AutoAddGauge();

	void UltimateGaugeChanged (const float InUltimateGaugePercentage) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float MaxUltimateGauge = 2000.f;
	
	UPROPERTY()
	float UltimateGauge = 0.f;

	UPROPERTY()
	float UltimateGaugePercentage;
	
	UPROPERTY()
	bool bUltimateAvailable = false;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	float AutoAddUltimateGaugeAmount = 5.f;

	UPROPERTY()
	FTimerHandle AutoAddUltimateGaugeTimerHandle;
};
