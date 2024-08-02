#pragma once

#include "CoreMinimal.h"
#include "Enums/TeamID.h"
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

	// 캐릭터가 죽으면 실행할 함수. 주로 HPComponent에서 HP가 0이되면 실행. 오버라이드해서 사용할 것
	virtual void CharacterDeath();
	// 캐릭터를 부활시키기 위해 실행 할 함수. 
	virtual void CharacterRevive();
	// TeamID에 따른 콜리전 프로필 세팅
	virtual void SetCollisionProfileByTeam(ETeamID InTeamID);

	// 캐릭터가 죽으면 컨트롤러에 알리는 함수
	virtual void NotifyCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot);

	// TeamID Getter
	UFUNCTION()
	FORCEINLINE ETeamID GetTeamID() const{ return TeamID; }

	// 체력 관리하는 액터 컴포넌트
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHPComponent> HPComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterBase", meta=(AllowPrivateAccess = "true"))
	ETeamID TeamID = ETeamID::ETI_Team1;
	
};
