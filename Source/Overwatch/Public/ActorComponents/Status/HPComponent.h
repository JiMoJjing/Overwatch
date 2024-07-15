#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HPComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, InCurrentHP, float, InMaxHP);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UHPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHPComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float TakeDamage(float InDamageAmount, bool& IsDeath);
	float TakeHeal(float InHealAmount);
	void CharacterDeath() const;
	void CharacterRevive();
	void HPChanged() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHPChanged OnHPChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxHP = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHP;

	
	
};
