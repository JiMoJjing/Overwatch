#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/AbilityType.h"
#include "Enums/AbilityState.h"
#include "AbilityManagementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityStarted, EAbilityType, InAbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityFinished, EAbilityType, InAbilityType);

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
	// ��� ������̸� ����� Ÿ��, ������� ����� ������ EAbilityType::EAT_None ��ȯ
	UFUNCTION(BlueprintCallable)
	EAbilityType GetActiveAbilityType() { return ActiveAbilityType; }

	// ���� ��� ������̸� true, ������ false
	FORCEINLINE bool IsAbilityActive() { return ActiveAbilityType != EAbilityType::EAT_None; }

	void NotifyAbilityStart(EAbilityType InAbilityType);
	void NotifyAbilityFinish(EAbilityType InAbilityType);
	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityStarted OnAbilityStarted;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityFinished OnAbilityFinished;

private:
	void AbilityStarted(EAbilityType InAbilityType);

	void AbilityFinished(EAbilityType InAbilityType);

private:
	EAbilityType ActiveAbilityType;
};
