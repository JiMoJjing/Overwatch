#pragma once

#include "CoreMinimal.h"
#include "Projectiles/ProjectileBase.h"
#include "TwoSphereProjectileBase.generated.h"

UCLASS()
class OVERWATCH_API ATwoSphereProjectileBase : public AProjectileBase
{
	GENERATED_BODY()

public:
	ATwoSphereProjectileBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(const FVector& StartLocation, const FVector& Direction) override;

protected:
	virtual void Deactivate() override;
	
	UFUNCTION()
	virtual void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void SetCollisionProfileByTeam(ETeamID TeamID) override;

	virtual void Deflected(APawn* NewInstigator, const FVector& Direction) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile_OverlapSphere", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> OverlapSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile_OverlapSphere", meta = (AllowPrivateAccess = "true"))
	float OverlapSphereRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile_OverlapSphere", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	
};
