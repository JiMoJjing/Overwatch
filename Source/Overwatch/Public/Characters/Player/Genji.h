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
	/** ���1 */
	virtual void AbilityOne() override;

	/** ���2 */
	virtual void AbilityTwo() override;

	/** ���3 */
	virtual void AbilityThree() override;

	/** �� ���� (�� Ŭ��) */
	virtual void PrimaryFire() override;

	/** ���� ���� (�� Ŭ��) */
	virtual void SecondaryFire() override;

	/** ������ */
	virtual void Reload() override;

	/** ���� ���� */
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
