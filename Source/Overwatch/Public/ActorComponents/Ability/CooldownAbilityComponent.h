#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "CooldownAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownTimeChanged, float, InCooldownTime, float, InCooldownDuration);

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

protected:
	// Cooldown 시작
	virtual void CooldownStart();
	
	// bHasCooldownWidget이 true면 SetTimer에서 호출 될 함수 ( 0.1초 간격 )
	virtual void CooldownTimerTick();

	// bHasCooldownWidget이 false면 SetTimer에서 호출 or CooldownTimerTick에서 NowCooldownTime이 0이되면 호출
	virtual void CooldownEnd();

	// CooldownWidget 사용 시 Delegate Broadcast 하는 함수 
	void CooldownTimeChanged(const float InNowCooldownTime);

public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownTimeChanged OnCooldownTimeChanged;

protected:
	// 쿨다운 위젯 사용 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	bool bHasCooldownWidget = false;

	// 쿨다운 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	float CooldownDuration;

	// 쿨다운 TimerHandle
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	// 현재 쿨타임
	float NowCooldownTime;
};
