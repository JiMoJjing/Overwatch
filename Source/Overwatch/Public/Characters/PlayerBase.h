#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "InputActionValue.h"
#include "PlayerBase.generated.h"


UCLASS()
class OVERWATCH_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** InputActions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveBackwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* AbilityThreeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* PrimaryFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* SecondaryFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* QuickMeleeAction;


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

	virtual void StopMovement();


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
	virtual void Reload();

	/** ���� ���� */
	virtual void QuickMelee();

};
