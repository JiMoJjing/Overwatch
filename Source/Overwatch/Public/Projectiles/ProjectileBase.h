#pragma once

#include "CoreMinimal.h"
#include "Enums/TeamID.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

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

	virtual void Activate(const FVector& StartLocation, const FVector& Direction);

	virtual void Deflected(APawn* NewInstigator, const FVector& Direction);

	void SetProjectileAmmoComponent(UProjectileAmmoComponent* InComponent);
	
protected:
	virtual void Deactivate();

	UFUNCTION()
	virtual void OnHitSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void LifeSpanTimerRestart();
	
public:
	virtual void SetCollisionProfileByTeam(ETeamID TeamID);

protected:
	// 벽 판정용 SphereComponent 인게임에서 여러 투사체들은 벽에대한 판정과 캐릭터에 대한 판정이 다르다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> HitSphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	// ProjectileSphereComponent's Radius
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "100.0"))
	float HitSphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "20000.0"))
	float ProjectileInitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "20000.0"))
	float ProjectileMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "3.0"))
	float ProjectileGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "10.0"))
	float LifeSpan = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Damage = 0.f;

	UPROPERTY()
	FTimerHandle LifeSpanTimerHandle;

	UPROPERTY()
	FName HitSphereCollisionProfileName = FName(TEXT("Team1ProjectileHit"));

	UPROPERTY()
	TWeakObjectPtr<UProjectileAmmoComponent> ProjectileAmmoComponent;
};
