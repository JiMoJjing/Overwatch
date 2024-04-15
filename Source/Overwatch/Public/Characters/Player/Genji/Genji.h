#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/TimelineComponent.h"
#include "Genji.generated.h"


class UProjectilePoolComponent;
class UGenji_PrimaryFireComponent;
class UGenji_SecondaryFireComponent;
class USwiftStrikeComponent;
class UAbilityComponent;

UCLASS()
class OVERWATCH_API AGenji : public APlayerBase
{
	GENERATED_BODY()
public:
	AGenji();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void Jump() override;

	virtual void StopJumping() override;

	void JumpCountReset();

public:
	virtual void MovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPrevCustomMovementMode) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UProjectilePoolComponent* GetProjectilePoolComponent() const { return ProjectilePoolComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGenji_PrimaryFireComponent* GetPrimaryFireComponent() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGenji_SecondaryFireComponent* GetSecondaryFireComponent() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USwiftStrikeComponent* GetSwiftStrikeComponent() const;


protected:
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	virtual void AbilityThree() override;
	virtual void PrimaryFire() override;
	virtual void SecondaryFire() override;
	virtual void Reloading() override;
	virtual void QuickMelee() override;

private:
	UFUNCTION()
	void SecondJump();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectilePoolComponent> ProjectilePoolComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> PrimaryFireComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> SecondaryFireComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> SwiftStrikeComponent;
	
	UPROPERTY()
	uint32 JumpCount;

};
