#pragma once

#include "CoreMinimal.h"
#include "Enums/TeamID.h"
#include "GameFramework/Actor.h"
#include "SwiftStrikeCollider.generated.h"

class UBoxComponent;
class UNiagaraSystem;

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

	void Activate() const;
	void Deactivate() const;
	
	
private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnSwiftStrikeHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const;
	void SetCollisionProfileByTeam(ETeamID TeamID);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SwiftStrikeCollider", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> SwiftStrikeHitEffect;
	
};
