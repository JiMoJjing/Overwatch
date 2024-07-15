#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/Ability/CooldownAbilityComponent.h"
#include "Components/TimelineComponent.h"
#include "Genji_SwiftStrikeComponent.generated.h"

class UAnimMontage;
class ASwiftStrikeCollider;
class AGenji;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERWATCH_API UGenji_SwiftStrikeComponent : public UCooldownAbilityComponent
{
	GENERATED_BODY()
public:	
	UGenji_SwiftStrikeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Montage NotifyTick ���� ȣ��Ǵ� ��ǳ�� ���� ������Ʈ �Լ�
	UFUNCTION()
	void SwiftStrikeUpdate(float DeltaTime);

	// Montage Notify ���� ȣ��Ǵ� ��ǳ�� ���� �Լ�
	UFUNCTION()
	void SwiftStrikeMontageFinished();

protected:
	// AbilityManagement�� ��� Ȱ��ȭ, ��Ȱ��ȭ �ϴ� �Լ� override
	virtual void StartAbility() override;
	virtual void FinishAbility() override;

private:
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

private:
	UPROPERTY()
	TObjectPtr<AGenji> GenjiRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	float SwiftStrikeDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	float SwiftStrikeSpeed;

	FVector SwiftStrikeStartLocation = FVector::ZeroVector;

	FVector SwiftStrikeEndLocation = FVector::ZeroVector;

	FVector SwiftStrikeHitNormalProjection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	float HitNormalProjectionInterpSpeed = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> SwiftStrikeCapsuleSizeCurveFloat;

	UPROPERTY()
	FOnTimelineFloat OnSwiftStrikeCapsuleSizeTimelineUpdate;

	UPROPERTY()
	FOnTimelineEvent OnSwiftStrikeCapsuleSizeTimelineFinished;

	UPROPERTY()
	FTimeline SwiftStrikeCapsuleSizeTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	FVector2D CapsuleSize2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	FVector2D SwiftStrikeCapsuleSize2D;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability_Genji", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASwiftStrikeCollider> SwiftStrikeColliderClass;

	UPROPERTY()
	TObjectPtr<ASwiftStrikeCollider> SwiftStrikeCollider;
	
	bool bSwiftStrike;
};
