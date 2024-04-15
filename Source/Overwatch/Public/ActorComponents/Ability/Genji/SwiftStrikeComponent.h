#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/AbilityComponent.h"
#include "Components/TimelineComponent.h"
#include "SwiftStrikeComponent.generated.h"

class UAnimMontage;
class ASwiftStrikeCollider;
class AGenji;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERWATCH_API USwiftStrikeComponent : public UAbilityComponent
{
	GENERATED_BODY()
public:	
	USwiftStrikeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void UseAbility() override;

	// Montage NotifyTick 에서 호출되는 질풍참 방향 업데이트 함수
	UFUNCTION()
	void SwiftStrikeUpdate(float DeltaTime);

	// Montage Notify 에서 호출되는 질풍참 종료 함수
	UFUNCTION()
	void SwiftStrikeMontageFinished();

protected:
	// AbilityManagement에 기술 활성화, 비활성화 하는 함수 override
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

private:
	// 질풍참 시작
	void SwiftStrikeStarted();
	// 질풍참 마무리
	//void SwiftStrikeFinished();
	// 질풍참 시작 위치 설정
	void SetSwiftStrikeStartLocation();
	// 질풍참 도착 위치 설정
	void SetSwiftStrikeEndLocation();
	// 질풍참 시작 세팅
	void SwiftStrikeStartSetting();
	// 질풍참 마무리 세팅
	void SwiftStrikeFinishSetting();

	// 질풍참 도중 캡슐이 Hit시 (벽에 닿았을 때) 벽에대한 투영벡터 반영시킬 바인드함수
	UFUNCTION()
	void OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// 캡슐 사이즈 조절 타임라인 세팅
	UFUNCTION()
	void SwiftStrikeCapsuleSizeTimelineSettings();

	// 캡슐 사이즈 조절 타임라인 바인드된 함수
	UFUNCTION()
	void SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha);

	// 캡슐 사이즈 조절 타임라인 마무리 함수
	UFUNCTION()
	void SwiftStrikeCapsuleSizeTimelineFinished();

	// 질풍참 몽타주가 캔슬되었을 때
	UFUNCTION()
	void SwiftStrikeMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TimeDilationValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SwiftStrikeDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SwiftStrikeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SwiftStrikeMontage;

	FVector SwiftStrikeStartLocation = FVector::ZeroVector;

	FVector SwiftStrikeEndLocation = FVector::ZeroVector;

	FVector SwiftStrikeHitNormalProjection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float HitNormalProjectionInterpSpeed = 10.f;

	bool bSwiftStrike;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> SwiftStrikeCapsuleSizeCurveFloat;

	UPROPERTY()
	FOnTimelineFloat OnSwiftStrikeCapsuleSizeTimelineUpdate;

	UPROPERTY()
	FOnTimelineEvent OnSwiftStrikeCapsuleSizeTimelineFinished;

	UPROPERTY()
	FTimeline SwiftStrikeCapsuleSizeTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D CapsuleSize2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector2D SwiftStrikeCapsuleSize2D;

	UPROPERTY()
	uint32 JumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASwiftStrikeCollider> SwiftStrikeColliderClass;

	UPROPERTY()
	TObjectPtr<ASwiftStrikeCollider> SwiftStrikeCollider;
};
