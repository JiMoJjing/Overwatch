#pragma once

#include "CoreMinimal.h"
#include "AmmoComponent.h"
#include "ProjectileAmmoComponent.generated.h"


class AProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UProjectileAmmoComponent : public UAmmoComponent
{
	GENERATED_BODY()

public:
	UProjectileAmmoComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ActivateProjectile(const FVector& StartLocation, const FVector& Direction);

	UFUNCTION()
	void DeactivateProjectile(AProjectileBase* ProjectileBase);

private:
	void InitializePool();
	
	UFUNCTION()
	AProjectileBase* Expand();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo_Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo_Projectile", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "1000"))
	int32 PoolSize;

	UPROPERTY()
	TArray<AProjectileBase*> ProjectilePool;
};
