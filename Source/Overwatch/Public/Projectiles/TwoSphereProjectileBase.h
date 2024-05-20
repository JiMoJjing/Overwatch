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

protected:
	UFUNCTION()
	virtual void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> OverlapSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float OverlapSphereRadius = 10.f;
};
