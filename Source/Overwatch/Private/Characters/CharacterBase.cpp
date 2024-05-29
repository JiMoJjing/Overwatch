#include "Characters/CharacterBase.h"

#include "Engine/DamageEvents.h"
#include "Interfaces/IApplyDamageSuccessHandler.h"
#include "ActorComponents/Status/HPComponent.h"
#include "Utilities/CLog.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HPComponent = CreateDefaultSubobject<UHPComponent>(TEXT("HPComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	bool bIsHeadShot = false;
	// PointDamage의 경우 헤드샷 검사
	if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if(PointDamageEvent->HitInfo.BoneName == FName("head") || PointDamageEvent->HitInfo.BoneName == FName("neck_01"))
		{
			bIsHeadShot = true;
			Damage *= 2;
		}
	}

	// HPComponent 에서 데미지 처리 ( 방어구등 데미지에 영향주거나 남은 체력에 따른 실제로 준 데미지 계산 후 적용 )
	if(HPComponent)
	{
		Damage = HPComponent->TakeDamage(Damage);
	}
	
	// 데미지를 입었다면 Instigator의 ApplyDamage가 성공했다고 판단, 그 함수 호출
	if(EventInstigator != nullptr)
	{
		if(IIApplyDamageSuccessHandler* IApplyDamageSuccessHandler = Cast<IIApplyDamageSuccessHandler>(EventInstigator->GetPawn()))
		{
			IApplyDamageSuccessHandler->Execute_ApplyDamageSuccess(EventInstigator->GetPawn(), Damage, bIsHeadShot);
		}
	}
	
	return Damage;
}

float ACharacterBase::TakeHeal(float InHealAmount, AController* EventInstigator, AActor* HealCauser)
{
	float HealAmount = InHealAmount;

	if(HPComponent)
	{
		HealAmount = HPComponent->TakeHeal(HealAmount);
	}
	
	return HealAmount;
}

void ACharacterBase::CharacterDeath()
{
	
}
