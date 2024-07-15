#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/AbilityComponent.h"
#include "AmmoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, CurrentAmmo);

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERWATCH_API UAmmoComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:	
	UAmmoComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UseAbility() override;

	// ź �Ҹ�
	void ConsumeAmmo(int32 InAmount);
 
	// ������ ( ������ Montage�� AnimNotify���� ȣ�� )
	void Reload();

	// ź�� 0�̻����� üũ
	FORCEINLINE bool CanFire() const { return CurrentAmmo > 0; };

	// �ִ� ź ��
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	FORCEINLINE bool IsAmmoFull() const { return CurrentAmmo == MaxAmmo; }

protected:
	virtual void StartAbility() override;
	virtual void FinishAbility() override;
	virtual void OnOtherAbilityFinished(EAbilityType InAbilityType) override;

	virtual void PlayReloadingMontage();
	virtual void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted) override;

	void AmmoChanged() const;
public:
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;
	
	UPROPERTY()
	int32 CurrentAmmo;

	bool bBufferedInput = false;

};
