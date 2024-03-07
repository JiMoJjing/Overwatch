#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerBase.h"
#include "Components/TimelineComponent.h"
#include "Genji.generated.h"

/**
 * 
 */
UCLASS()
class OVERWATCH_API AGenji : public APlayerBase
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float TimeDilationValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float SwiftStrikeRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float SwiftStrikeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCurveFloat* SwiftStrikeCurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* SwiftStrikeMontage;

	FOnTimelineFloat OnSwiftStrikeTimelineUpdate;

	FOnTimelineEvent OnSwiftStrikeTimelineFinished;

	FTimeline SwiftStrikeTimeline;
	
	FVector SwiftStrikeStartLocation;

	FVector SwiftStrikeEndLocation;

	FVector SwiftStrikeHitNormalProjection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float HitNormalProjectionInterpSpeed = 10.f;

	bool bSwiftStrike;

	bool bSwiftStrikeHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCurveFloat* SwiftStrikeCapsuleSizeCurveFloat;

	FOnTimelineFloat OnSwiftStrikeCapsuleSizeTimelineUpdate;

	FOnTimelineEvent OnSwiftStrikeCapsuleSizeTimelineFinished;

	FTimeline SwiftStrikeCapsuleSizeTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector2D CapsuleSize2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FVector2D SwiftStrikeCapsuleSize2D;
	
	bool bSecondJump;
	
public:
	AGenji();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void Jump() override;

	virtual void StopJumping() override;


protected:
	/** 기술1 */
	virtual void AbilityOne() override;

	/** 기술2 */
	virtual void AbilityTwo() override;

	/** 기술3 */
	virtual void AbilityThree() override;

	/** 주 무기 (좌 클릭) */
	virtual void PrimaryFire() override;

	/** 보조 무기 (우 클릭) */
	virtual void SecondaryFire() override;

	/** 재장전 */
	virtual void Reload() override;

	/** 근접 공격 */
	virtual void QuickMelee() override;


private:
	UFUNCTION()
		void SwiftStrike();

	UFUNCTION()
		void SwiftStrikeTimelineSettings();

	UFUNCTION()
		void SetSwiftStrikeStartLocation();

	UFUNCTION()
		void SetSwiftStrikeEndLocation();

	UFUNCTION()
		void SwiftStrikeTimelineStarted();

	UFUNCTION()
		void SwiftStrikeTimelineUpdate(float Alpha);

	UFUNCTION()
		void SwiftStrikeTimelineFinished();

	UFUNCTION()
		void OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void SwiftStrikeCapsuleSizeTimelineSettings();

	UFUNCTION()
		void SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha);

	UFUNCTION()
		void SwiftStrikeCapsuleSizeTimelineFinished();
};
