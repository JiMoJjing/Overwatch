#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AbilityType.h"
#include "Enums/AbilityState.h"
#include "AbilityManagementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityActivated, EAbilityType, InAbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityDeactivated, EAbilityType, InAbilityType);

class UAbilityComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERWATCH_API UAbilityManagementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityManagementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 기술 사용중이면 기술의 타입, 사용중인 기술이 없으면 EAbilityType::EAT_None 반환
	UFUNCTION(BlueprintCallable)
	EAbilityType GetAbilityType() { return ActiveAbilityType; }

	// 현재 기술 사용중이면 true, 없으면 false
	FORCEINLINE bool IsAbilityActivate() { return ActiveAbilityType != EAbilityType::EAT_None; }

	void ActivateAbility(EAbilityType InAbilityType);
	void DeactivateAbility(EAbilityType InAbilityType);


	UPROPERTY(BlueprintAssignable)
	FOnAbilityActivated OnAbilityActivated;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDeactivated OnAbilityDeactivated;

private:
	void AbilityActivated(EAbilityType InAbilityType);

	void AbilityDeactivated(EAbilityType InAbilityType);

private:
	UPROPERTY()
	TObjectPtr<UAbilityComponent> ActiveAbilityComponent;

	EAbilityType ActiveAbilityType;
};
