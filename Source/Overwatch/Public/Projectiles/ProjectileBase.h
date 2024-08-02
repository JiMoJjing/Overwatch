#pragma once

#include "CoreMinimal.h"
#include "Enums/TeamID.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;
class UProjectileAmmoComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;

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

	void SetProjectileAmmoComponent(UProjectileAmmoComponent* InComponent);
	
	virtual void Activate(const FVector& StartLocation, const FVector& Direction);
	
protected:
	virtual void Deactivate();

	UFUNCTION()
	virtual void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void LifeSpanTimerStart();
	void LifeSpanTimerEnd();
	void LifeSpanTimerRestart();
	void SpawnHitActorEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;
	void SpawnHitWallEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;
	
	virtual void SetCollisionProfileByTeam(ETeamID TeamID);

	virtual void Deflected(APawn* NewInstigator, const FVector& Direction);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> HitSphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "100.0"))
	float HitSphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "20000.0"))
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "20000.0"))
	float ProjectileMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "3.0"))
	float ProjectileGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "10.0"))
	float LifeSpan = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true"))
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> HitActorEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileBase", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> HitWallEffect;
	
	
	UPROPERTY()
	FTimerHandle LifeSpanTimerHandle;
	
	UPROPERTY()
	TWeakObjectPtr<UProjectileAmmoComponent> ProjectileAmmoComponent;
};
