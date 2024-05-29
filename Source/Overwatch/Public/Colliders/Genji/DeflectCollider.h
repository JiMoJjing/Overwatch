#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeflectCollider.generated.h"

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


private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;
};
