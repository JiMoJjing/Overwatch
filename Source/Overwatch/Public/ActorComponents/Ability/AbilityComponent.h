#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AbilityType.h"
#include "Enums/AbilityState.h"
#include "AbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityStateChanged, uint8, InAbilityState);

class UAbilityManagementComponent;
class APlayerBase;

USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	TObjectPtr<UTexture> AbilityTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityData")
	FKey InputKey;
	
};

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
	UFUNCTION()
	FORCEINLINE EAbilityType GetAbilityType() const { return AbilityType; }
	
	UFUNCTION()
	FORCEINLINE UTexture* GetAbilityTexture() const { return AbilityData.AbilityTexture; }
	
	UFUNCTION()
	FName GetInputKeyName() const { return AbilityData.InputKey.GetFName(); }
	
	UFUNCTION()
	virtual void UseAbility();
	
protected:
	// ����� ���� �������� üũ
	bool CanUseAbility();

	// ���� ������� ����� ĵ���� �� �ִ��� üũ
	bool CanCancelAbility();


	//����� ���۵� ��, ���� �� ȣ�� �ǵ��� ������ ��
	virtual void StartAbility();
	virtual void FinishAbility();

	/**
	* �ٸ� ����� ������ ���� ������ �� AbilityManagementComponent���� �ش� ����� Ÿ���� �ѷ��ִ� DELEGATE�� ���ε� �� �Լ�.
	* �ٸ� ����� ���� �ش� ����� AbilityState�� �ٲ� ��� �� �Լ��鿡 �����ϸ� �ȴ�.
	*/
	UFUNCTION()
	virtual void OnOtherAbilityStarted(EAbilityType InAbilityType);
	UFUNCTION()
	virtual void OnOtherAbilityFinished(EAbilityType InAbilityType);

	// FOnAbilityStateChanged ��������Ʈ Broadcast �Լ�
	void AbilityStateChanged() const;

	// AbilityMontage ĵ���Ǹ� ȣ�� �� �Լ�
	UFUNCTION()
	virtual void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);
	
public:
	UFUNCTION()
	virtual void AbilityWidgetInit();

	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityStateChanged OnAbilityStateChanged;
	
protected:
	// AbilityManagementComponent Reference
	UPROPERTY()
	TObjectPtr<UAbilityManagementComponent> AbilityManagementComponent;

	// ���� �� ����� ����
	UPROPERTY(BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = EAbilityState))
	uint8 AbilityState = 0;
	
	// �� ����� Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	// ĵ�� ������ ��� ���
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 CancelableAbilityTypes = 0;

	// �� Ability�� Available�� false�� ����� �����
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = "Ability", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = EAbilityType))
	uint8 MakeUnavailableAbilityTypes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Ability_AbilityData", meta = (AllowPrivateAccess = "true"))
	FAbilityData AbilityData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Ability_AbilityData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AbilityMontage;
};
