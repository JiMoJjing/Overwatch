#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/NPCBase.h"
#include "TrainingBot.generated.h"

class UProjectileAmmoComponent;

UCLASS()
class OVERWATCH_API ATrainingBot : public ANPCBase
{
	GENERATED_BODY()

public:
	ATrainingBot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();

protected:
	virtual void CharacterDeath() override;

	virtual void CharacterRevive() override;

	void StartFire();

	void StopFire();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileAmmoComponent> ProjectileAmmoComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bFireMode = false;
	
	UPROPERTY()
	FTimerHandle AutoFireTimerHandle;
};
