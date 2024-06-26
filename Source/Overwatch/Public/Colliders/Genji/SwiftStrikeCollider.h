#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwiftStrikeCollider.generated.h"

class UBoxComponent;

UCLASS()
class OVERWATCH_API ASwiftStrikeCollider : public AActor
{
	GENERATED_BODY()
public:	
	ASwiftStrikeCollider();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SwiftStrikeStarted() const;
	void SwiftStrikeFinished() const;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;
};
