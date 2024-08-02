#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/UltimateAbilityComponent.h"
#include "Genji_DragonbladeComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDragonbladeActiveTimeChanged, float, DragonbladeActiveCurrentTime);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UGenji_DragonbladeComponent : public UUltimateAbilityComponent
{
	GENERATED_BODY()

public:
	UGenji_DragonbladeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void StartAbility() override;
	virtual void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted) override;
	
private:
	void DragonbladeDrawingStart();
	void DragonbladeDrawingFinish();
	
	void DragonbladeSheathingStart();
	void DragonbladeSheathingFinish();

	void DragonbladeActiveTimerStart();
	void DragonbladeActiveTimerTick();
	void DragonbladeActiveTimerEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Dragonblade", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DragonbladeDrawingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Dragonblade", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DragonbladeSheathingMontage;

	UPROPERTY()
	FTimerHandle DragonbladeActiveTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Dragonblade", meta = (AllowPrivateAccess = "true"))
	float DragonbladeActiveTime = 6.f;

	float DragonbladeActiveCurrentTime = 0.f;
};
