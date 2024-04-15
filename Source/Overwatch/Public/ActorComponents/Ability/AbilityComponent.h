#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AbilityType.h"
#include "Enums/AbilityState.h"
#include "AbilityComponent.generated.h"




class UAbilityManagementComponent;
class APlayerBase;

UCLASS(Abstract)
class OVERWATCH_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	FORCEINLINE EAbilityType GetAbilityType() const { return AbilityType; }

	// 기술 사용 함수. 캐릭터의 입력에서 호출. 반드시 재정의
	UFUNCTION()
	virtual void UseAbility() PURE_VIRTUAL(UAbilityComponent::UseAbility, );


protected:
	// AbilityState Bitmask Add, Sub
	void AddAbilityState(EAbilityState InAbilityState);
	void SubAbilityState(EAbilityState InAbilityState);

	// AbilityState Is, IsNot
	FORCEINLINE bool IsAbilityState(EAbilityState InAbilityState) const { return AbilityState & static_cast<uint8>(InAbilityState); }
	FORCEINLINE bool IsNotAbilityState(EAbilityState InAbilityState) const { return !(AbilityState & static_cast<uint8>(InAbilityState)); }

	// 기술이 실행 가능한지 체크
	bool CanActivateAbility();

	// 현재 사용중인 기술을 캔슬할 수 있는지 체크
	bool CanCancelAbility();

	/**
	* AbilityManagementComponent에 해당 기술이 시작됐음과 끝났음을 알려주는 함수
	*/
	virtual void ActivateAbility();
	virtual void DeactivateAbility();

	/**
	* 다른 기술이 사용됐을 때와 끝났을 때 AbilityManagementComponent에서 해당 기술의 타입을 뿌려주는 DELEGATE에 바인드 될 함수.
	* 다른 기술에 의해 해당 기술의 AbilityState가 바뀔 경우 이 함수들에 구현하면 된다.
	*/
	UFUNCTION()
	virtual void OnAbilityActivated(EAbilityType InAbilityType);
	UFUNCTION()
	virtual void OnAbilityDeactivated(EAbilityType InAbilityType);


	// Cooldown 시작
	virtual void CooldownStart();

	// bHasCooldownWidget이 true면 SetTimer에서 호출 될 함수 ( 0.1초 간격 )
	virtual void CooldownTimerTick();

	// bHasCooldownWidget이 false면 SetTimer에서 호출 or CooldownTimerTick에서 NowCooldownTime이 0이되면 호출
	virtual void CooldownEnd();
		
protected:
	// AbilityManagementComponent Reference
	UPROPERTY()
	TObjectPtr<UAbilityManagementComponent> AbilityManagementComponent;

	// PlayerBase Reference
	UPROPERTY()
	TObjectPtr<APlayerBase> PlayerBase;

	// 이 기술의 타입
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	// 현재 이 기술의 상태
	UPROPERTY(BlueprintReadOnly)
	uint8 AbilityState = 0;

	// 캔슬 가능한 기술 목록
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 CancelableAbilityTypes = 0;

	// 이 Ability의 Available을 false로 만드는 기술들
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 MakeUnavailableAbilityTypes = 0;

	// 쿨다운 사용 여부
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	bool bUseCooldown = true;

	// 쿨다운 위젯 사용 여부
	UPROPERTY(EditAnywhere, Category = "Cooldown", meta = (EditCondition = "bUseCooldown"))
	bool bHasCooldownWidget = false;

	// 쿨다운 시간
	UPROPERTY(EditAnywhere, Category = "Cooldown", meta = (EditCondition = "bUseCooldown"))
	float CooldownDuration;

	// 쿨다운 TimerHandle
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	// 현재 쿨타임
	float NowCooldownTime;
};
