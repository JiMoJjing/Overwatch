#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AbilityType.h"
#include "Enums/AbilityState.h"
#include "AbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityStateChanged, uint8, InAbilityState);

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

	// AbilityType Getter
	FORCEINLINE EAbilityType GetAbilityType() const { return AbilityType; }

	// AbilityTexture Getter
	UFUNCTION()
	FORCEINLINE UTexture* GetAbilityTexture() const {return AbilityTexture;}
	
	// 기술 사용 함수. 캐릭터의 입력에서 호출. 하위 클래스에서 재정의
	// UFUNCTION()
	// virtual void UseAbility() PURE_VIRTUAL(UAbilityComponent::UseAbility, );

	UFUNCTION()
	virtual void UseAbility();
	
protected:
	// 기술이 실행 가능한지 체크
	bool CanActivateAbility();

	// 현재 사용중인 기술을 캔슬할 수 있는지 체크
	bool CanCancelAbility();

	/**
	* 기술이 시작될 때, 끝날 때 호출 되도록 구현할 것
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

	void AbilityStateChanged() const;
	
public:
	UFUNCTION()
	virtual void AbilityWidgetInit();

	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityStateChanged OnAbilityStateChanged;
	
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

	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture> AbilityTexture;
};
