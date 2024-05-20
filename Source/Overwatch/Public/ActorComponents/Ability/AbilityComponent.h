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
	
	// ��� ��� �Լ�. ĳ������ �Է¿��� ȣ��. ���� Ŭ�������� ������
	// UFUNCTION()
	// virtual void UseAbility() PURE_VIRTUAL(UAbilityComponent::UseAbility, );

	UFUNCTION()
	virtual void UseAbility();
	
protected:
	// ����� ���� �������� üũ
	bool CanActivateAbility();

	// ���� ������� ����� ĵ���� �� �ִ��� üũ
	bool CanCancelAbility();

	/**
	* ����� ���۵� ��, ���� �� ȣ�� �ǵ��� ������ ��
	*/
	virtual void ActivateAbility();
	virtual void DeactivateAbility();

	/**
	* �ٸ� ����� ������ ���� ������ �� AbilityManagementComponent���� �ش� ����� Ÿ���� �ѷ��ִ� DELEGATE�� ���ε� �� �Լ�.
	* �ٸ� ����� ���� �ش� ����� AbilityState�� �ٲ� ��� �� �Լ��鿡 �����ϸ� �ȴ�.
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

	// �� ����� Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	// ���� �� ����� ����
	UPROPERTY(BlueprintReadOnly)
	uint8 AbilityState = 0;

	// ĵ�� ������ ��� ���
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 CancelableAbilityTypes = 0;

	// �� Ability�� Available�� false�� ����� �����
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 MakeUnavailableAbilityTypes = 0;

	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture> AbilityTexture;
};
