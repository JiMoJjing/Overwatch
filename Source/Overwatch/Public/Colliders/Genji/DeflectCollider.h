#pragma once

#include "CoreMinimal.h"
#include "Enums/TeamID.h"
#include "GameFramework/Actor.h"
#include "DeflectCollider.generated.h"

class UGenji_DeflectComponent;
class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class OVERWATCH_API ADeflectCollider : public AActor
{
	GENERATED_BODY()

public:
	ADeflectCollider();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetGenji_DeflectComponent(UGenji_DeflectComponent* Genji_DeflectComponent);

	void Activate() const;
	void Deactivate() const;

	FVector GetDeflectDirection(const FVector& StartLocation) const;
	
private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetCollisionProfileByTeam(ETeamID TeamID);

	void PlayDeflectMontage() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Deflect", meta = (AllowPrivateAccess = "true"))
	FVector BoxExtend = FVector(32.f, 80.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Genji_Deflect", meta = (AllowPrivateAccess = "true"))
	FVector RelativeLocation = FVector(90.f, 0.f, 0.f);
	
	UPROPERTY()
	TWeakObjectPtr<UGenji_DeflectComponent> DeflectComponent;
};
