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

	// Montage NotifyTick ���� ȣ��Ǵ� ��ǳ�� ���� ������Ʈ �Լ�
	UFUNCTION()
	void SwiftStrikeUpdate(float DeltaTime);

	// Montage Notify ���� ȣ��Ǵ� ��ǳ�� ���� �Լ�
	UFUNCTION()
	void SwiftStrikeMontageFinished();

protected:
	// AbilityManagement�� ��� Ȱ��ȭ, ��Ȱ��ȭ �ϴ� �Լ� override
	virtual void ActivateAbility() override;
	virtual void DeactivateAbility() override;

private:
	// ��ǳ�� ����
	void SwiftStrikeStarted();
	// ��ǳ�� ������
	//void SwiftStrikeFinished();
	// ��ǳ�� ���� ��ġ ����
	void SetSwiftStrikeStartLocation();
	// ��ǳ�� ���� ��ġ ����
	void SetSwiftStrikeEndLocation();
	// ��ǳ�� ���� ����
	void SwiftStrikeStartSetting();
	// ��ǳ�� ������ ����
	void SwiftStrikeFinishSetting();

	// ��ǳ�� ���� ĸ���� Hit�� (���� ����� ��) �������� �������� �ݿ���ų ���ε��Լ�
	UFUNCTION()
	void OnSwiftStrikeComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// ĸ�� ������ ���� Ÿ�Ӷ��� ����
	UFUNCTION()
	void SwiftStrikeCapsuleSizeTimelineSettings();

	// ĸ�� ������ ���� Ÿ�Ӷ��� ���ε�� �Լ�
	UFUNCTION()
	void SwiftStrikeCapsuleSizeTimelineUpdate(float Alpha);

	// ĸ�� ������ ���� Ÿ�Ӷ��� ������ �Լ�
	UFUNCTION()
	void SwiftStrikeCapsuleSizeTimelineFinished();

	// ��ǳ�� ��Ÿ�ְ� ĵ���Ǿ��� ��
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
