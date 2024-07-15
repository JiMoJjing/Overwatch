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
	* ������ ���� ���� �Լ�
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

	// EAbilityType�� �Է¹޾� �ش��ϴ� Ability ������Ʈ ��ȯ
	UFUNCTION()
	FORCEINLINE UAbilityComponent* GetAbilityComponent(EAbilityType InAbilityType) const;
	
	
	// MovementMode �ٲ� �� ���� �� �Լ� override�ؼ� ����ϱ�
	UFUNCTION()
	virtual void MovementModeChanged(ACharacter* InCharacter, EMovementMode InPrevMovementMode, uint8 InPrevCustomMovementMode);
	



	/**
	* InputAction�� ���ε� �� �Լ��� Child Class���� override �Ͽ� ����� ��
	*/
protected:
	/** ��ȣ�ۿ� */
	void Interaction();

	/** ���1 */
	virtual void AbilityOne();

	/** ���2 */
	virtual void AbilityTwo();

	/** ���3 */
	virtual void AbilityThree();

	/** �� ���� (�� Ŭ��) */
	virtual void PrimaryFire();

	/** ���� ���� (�� Ŭ��) */
	virtual void SecondaryFire();

	/** ������ */
	virtual void Reloading();

	/** ���� ���� */
	virtual void QuickMelee();

public:
	// ApplyDamage�� �������� ��� TakeDamage�Լ����� ȣ��
	virtual void ApplyDamageSuccess_Implementation(float Damage, bool bIsHeadShot) override;
	
	/**
	 * TraceUnderCrosshair : ũ�ν���� ��ġ���� ������ �������� LineTrace�Ͽ� HitResult�� ������ �� Result�� Location ��ȯ, ������ LineTraceEndLocation��ȯ
	 * @param TraceDistance Trace range
	 * @param OutHitResult HitResult
	 * @param OutHitLocation HitLocation
	 * @param InCollisionChannel TraceChannel 
	 * @return 
	 */
	bool TraceUnderCrosshair(const float TraceDistance, FHitResult& OutHitResult, FVector& OutHitLocation, const ECollisionChannel InCollisionChannel);
	/**
	 * GetDirectionToCrosshair : ���ϴ� ������ġ���� Crosshair�������� Trace�� ��ġ�� ���ϴ� ���⺤�� ���ϴ� �Լ�(ex. �ѱ����⿡�� Crosshair��ġ���� ���⺤��)
	 * @param StartLocation ������ġ
	 * @param OutDirection ��� ���⺤�� ���� ���ͺ���
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

	/** Ability ���� ( ���� ������� ��� �� ) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilityManagementComponent> AbilityManagementComponent;

	// ���� Ŭ�������� Create�� ��
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

