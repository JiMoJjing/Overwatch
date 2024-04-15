#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class OVERWATCH_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:	
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Activate(const FVector& StartLocation, const FVector& Direction);

	UFUNCTION()
	FORCEINLINE bool CanActivate() { return bCanActivate; }

protected:
	UFUNCTION()
	virtual void Deactivate();

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// SphereComponent for projectile MovementComponent. Add Another Collision Component for Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> ProjectileSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	// ProjectileSphereComponent's Radius
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "100.0"))
	float ProjectileSphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "20000.0"))
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "20000.0"))
	float ProjectileMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "3.0"))
	float ProjectileGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "10.0"))
	float LifeSpan = 3.0f;

	UPROPERTY()
	bool bCanActivate = true;

	UPROPERTY()
	FTimerHandle LifeSpanTimerHandle;
};
