#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "InputActionValue.h"
#include "PlayerBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAbilityManagementComponent;
class UAmmoComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class OVERWATCH_API APlayerBase : public ACharacterBase
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

public:
	UFUNCTION()
	virtual void MovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPrevCustomMovementMode);

	UFUNCTION()
	FORCEINLINE UAbilityManagementComponent* GetAbilityManagementComponent() const { return AbilityManagementComponent; }

	UFUNCTION()
	FORCEINLINE UAmmoComponent* GetAmmoComponent() const { return AmmoComponent; }


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
	bool TraceUnderCrosshair(float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, ECollisionChannel InCollisionChannel);
	bool GetDirectionToCrosshair(const FVector& StartLocation, FVector& OutDirection, ECollisionChannel InCollisionChannel);

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityManagementComponent> AbilityManagementComponent;

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
};

