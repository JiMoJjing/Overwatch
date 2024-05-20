#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/TimelineComponent.h"
#include "Genji.generated.h"


class UProjectilePoolComponent;
class UGenji_PrimaryFireComponent;
class UGenji_SecondaryFireComponent;
class UGenji_SwiftStrikeComponent;
class UGenji_DeflectComponent;
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

	/** AbilityComponent Getter 인데 캐릭터 전용으로 만든 AbilityComponent 하위 클래스 Getter임 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UProjectilePoolComponent* GetProjectilePoolComponent() const { return ProjectilePoolComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGenji_PrimaryFireComponent* GetGenji_PrimaryFireComponent() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGenji_SecondaryFireComponent* GetGenji_SecondaryFireComponent() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGenji_SwiftStrikeComponent* GetGenji_SwiftStrikeComponent() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGenji_DeflectComponent* GetGenji_DeflectComponent() const;

protected:
	virtual void AbilityOne() override;
	virtual void AbilityTwo() override;
	virtual void AbilityThree() override;
	virtual void PrimaryFire() override;
	virtual void SecondaryFire() override;
	virtual void Reloading() override;
	virtual void QuickMelee() override;

	virtual void ApplyDamageSuccess_Implementation(float Damage, bool bIsHeadShot) override;
	
private:
	void SecondJump();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectilePoolComponent> ProjectilePoolComponent;
	
	UPROPERTY()
	uint32 JumpCount = 0;

};
