#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "InputActionValue.h"
#include "Enums/AbilityType.h"
#include "Interfaces/IApplyDamageSuccessHandler.h"
#include "PlayerBase.generated.h"

class UAbilityComponent;
class UUltimateAbilityComponent;
class USpringArmComponent;
class UCameraComponent;
class UAbilityManagementComponent;
class UAmmoComponent;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApplyDamageSuccessEvent, bool, bIsHeadShot);

UCLASS()
class OVERWATCH_API APlayerBase : public ACharacterBase, public IIApplyDamageSuccessHandler
{
	GENERATED_BODY()

public:
	APlayerBase();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

	/**
	* 움직임 제어 관련 함수
	*/
protected:
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual void Jump() override;
	virtual void StopJumping() override;

public:
	virtual void StopMovement();

	virtual void CharacterDeath() override;
	virtual void NotifyCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot) override;
	
	/** Getter */
	UFUNCTION()
	FORCEINLINE UAmmoComponent* GetAmmoComponent() const { return AmmoComponent; }
	
	UFUNCTION()
	FORCEINLINE UAbilityManagementComponent* GetAbilityManagementComponent() const { return AbilityManagementComponent; }

	UFUNCTION()
	FORCEINLINE UUltimateAbilityComponent* GetUltimateAbilityComponent() const { return UltimateAbilityComponent; }

	// EAbilityType을 입력받아 해당하는 Ability 컴포넌트 반환
	UFUNCTION()
	FORCEINLINE UAbilityComponent* GetAbilityComponent(EAbilityType InAbilityType) const;
	
	
	// MovementMode 바뀔 때 실행 할 함수 override해서 사용하기
	UFUNCTION()
	virtual void MovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPrevCustomMovementMode);
	



	/**
	* InputAction에 바인드 된 함수들 Child Class에서 override 하여 사용할 것
	*/
protected:
	/** 상호작용 */
	void Interaction();

	/** 기술1 */
	virtual void AbilityOne();

	/** 기술2 */
	virtual void AbilityTwo();

	/** 기술3 */
	virtual void AbilityThree();

	/** 주 무기 (좌 클릭) */
	virtual void PrimaryFire();

	/** 보조 무기 (우 클릭) */
	virtual void SecondaryFire();

	/** 재장전 */
	virtual void Reloading();

	/** 근접 공격 */
	virtual void QuickMelee();

public:
	// ApplyDamage에 성공했을 경우 TakeDamage함수에서 호출
	virtual void ApplyDamageSuccess_Implementation(float Damage, bool bIsHeadShot) override;
	
	/**
	 * TraceUnderCrosshair : 크로스헤어 위치에서 투영된 방향으로 LineTrace하여 HitResult가 있으면 그 Result와 Location 반환, 없으면 LineTraceEndLocation반환
	 * @param TraceDistance Trace range
	 * @param OutHitResult HitResult
	 * @param OutHitLocation HitLocation
	 * @param InCollisionChannel TraceChannel 
	 * @return 
	 */
	bool TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, const ECollisionChannel InCollisionChannel);
	/**
	 * GetDirectionToCrosshair : 원하는 시작위치에서 Crosshair방향으로 Trace한 위치를 향하는 방향벡터 구하는 함수(ex. 총구방향에서 Crosshair위치로의 방향벡터)
	 * @param StartLocation 시작위치
	 * @param OutDirection 결과 방향벡터 받을 벡터변수
	 * @param InCollisionChannel TraceChannel
	 * @return 
	 */
	bool GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, const ECollisionChannel InCollisionChannel);

protected:
	void ApplyDamageSuccessEvent(bool bIsHeadShot) const;
	
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Ability 관리 ( 현재 사용중인 기술 등 ) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityManagementComponent> AbilityManagementComponent;

	// 하위 클래스에서 Create할 것
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> PrimaryFireComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> SecondaryFireComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityOneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> AbilityTwoComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUltimateAbilityComponent> UltimateAbilityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityComponent> QuickMeleeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAmmoComponent> AmmoComponent;
	
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** InputActions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveBackwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AbilityOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AbilityTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AbilityThreeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PrimaryFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SecondaryFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> QuickMeleeAction;

public:
	UPROPERTY(BlueprintAssignable)
	FOnApplyDamageSuccessEvent OnApplyDamageSuccessEvent;
};

