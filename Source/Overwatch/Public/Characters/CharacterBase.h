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

	// 데미지와 회복 처리
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeHeal(float InHealAmount, AController* EventInstigator, AActor* HealCauser);

	// 캐릭터가 죽으면 실행할 함수. 주로 HPComponent에서 HP가 0이되면 실행 오버라이드해서 사용할 것
	virtual void CharacterDeath();

	// 체력 관리하는 액터 컴포넌트
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPComponent> HPComponent;
	
};
