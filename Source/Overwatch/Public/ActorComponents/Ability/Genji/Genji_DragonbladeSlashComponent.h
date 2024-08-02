#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Genji_DragonbladeSlashComponent.generated.h"


class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UGenji_DragonbladeSlashComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()

public:
	UGenji_DragonbladeSlashComponent();

protected:
	virtual void BeginPlay() override;
	virtual void StartAbility() override;
	virtual void CooldownTimerEnd() override;

private:
	void PlayAbilityMontage();
	void DragonbladeSlashSweep();
	void SpawnDragonbladeHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;
	void SetCollisionProfileByTeam();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Dragonblade", meta = (AllowPrivateAccess = "true"))
	uint8 DragonbladeSlashDamage = 110.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Dragonblade", meta = (AllowPrivateAccess = "true"))
	FVector SlashSweepBoxExtend = FVector(300.f, 200.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Dragonblade", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> DragonbladeSlashHitEffect;
	
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> SlashSweepTraceChannel = ECC_GameTraceChannel9;
	
	uint8 DragonbladeSlashMontageSectionCount = 0;
};
