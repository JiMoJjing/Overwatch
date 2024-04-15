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

	// ��� ��� �Լ�. ĳ������ �Է¿��� ȣ��. �ݵ�� ������
	UFUNCTION()
	virtual void UseAbility() PURE_VIRTUAL(UAbilityComponent::UseAbility, );


protected:
	// AbilityState Bitmask Add, Sub
	void AddAbilityState(EAbilityState InAbilityState);
	void SubAbilityState(EAbilityState InAbilityState);

	// AbilityState Is, IsNot
	FORCEINLINE bool IsAbilityState(EAbilityState InAbilityState) const { return AbilityState & static_cast<uint8>(InAbilityState); }
	FORCEINLINE bool IsNotAbilityState(EAbilityState InAbilityState) const { return !(AbilityState & static_cast<uint8>(InAbilityState)); }

	// ����� ���� �������� üũ
	bool CanActivateAbility();

	// ���� ������� ����� ĵ���� �� �ִ��� üũ
	bool CanCancelAbility();

	/**
	* AbilityManagementComponent�� �ش� ����� ���۵����� �������� �˷��ִ� �Լ�
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


	// Cooldown ����
	virtual void CooldownStart();

	// bHasCooldownWidget�� true�� SetTimer���� ȣ�� �� �Լ� ( 0.1�� ���� )
	virtual void CooldownTimerTick();

	// bHasCooldownWidget�� false�� SetTimer���� ȣ�� or CooldownTimerTick���� NowCooldownTime�� 0�̵Ǹ� ȣ��
	virtual void CooldownEnd();
		
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

	// ��ٿ� ��� ����
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	bool bUseCooldown = true;

	// ��ٿ� ���� ��� ����
	UPROPERTY(EditAnywhere, Category = "Cooldown", meta = (EditCondition = "bUseCooldown"))
	bool bHasCooldownWidget = false;

	// ��ٿ� �ð�
	UPROPERTY(EditAnywhere, Category = "Cooldown", meta = (EditCondition = "bUseCooldown"))
	float CooldownDuration;

	// ��ٿ� TimerHandle
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	// ���� ��Ÿ��
	float NowCooldownTime;
};
