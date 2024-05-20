#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UHPComponent;

UCLASS()
class OVERWATCH_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FORCEINLINE UHPComponent* GetHPComponent() const {return HPComponent;}

	// �������� ȸ�� ó��
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeHeal(float InHealAmount, AController* EventInstigator, AActor* HealCauser);

	// ĳ���Ͱ� ������ ������ �Լ�. �ַ� HPComponent���� HP�� 0�̵Ǹ� ���� �������̵��ؼ� ����� ��
	virtual void CharacterDeath();

	// ü�� �����ϴ� ���� ������Ʈ
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPComponent> HPComponent;
	
};
