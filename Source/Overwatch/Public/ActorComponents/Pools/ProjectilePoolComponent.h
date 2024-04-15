#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectilePoolComponent.generated.h"


class AProjectileBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERWATCH_API UProjectilePoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProjectilePoolComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	virtual void ActivateProjectile(const FVector& StartLocation, const FVector& Direction);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> PooledProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "1000"))
	int32 PoolSize;

	UPROPERTY()
	TArray<AProjectileBase*> ProjectilePools;

	UPROPERTY()
	int32 PoolIndex = 0;
};
