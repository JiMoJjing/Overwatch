#include "Characters/CharacterBase.h"

#include "Engine/DamageEvents.h"
#include "Interfaces/IApplyDamageSuccessHandler.h"
#include "ActorComponents/Status/HPComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controllers/OverwatchPlayerController.h"
#include "Utilities/CLog.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HPComponent = CreateDefaultSubobject<UHPComponent>(TEXT("HPComponent"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionProfileByTeam(TeamID);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	bool bIsHeadShot = false, bIsDeath = false;
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
		Damage = HPComponent->TakeDamage(Damage, bIsDeath);
	}
	
	// 데미지를 입었다면 Instigator의 ApplyDamage가 성공했다고 판단, 그 함수 호출
	if(EventInstigator != nullptr)
	{
		if(IIApplyDamageSuccessHandler* IApplyDamageSuccessHandler = Cast<IIApplyDamageSuccessHandler>(EventInstigator->GetPawn()))
		{
			IApplyDamageSuccessHandler->Execute_ApplyDamageSuccess(EventInstigator->GetPawn(), Damage, bIsHeadShot);
		}
	}

	if(bIsDeath)
	{
		CharacterDeath();
		NotifyCharacterDeath(EventInstigator, DamageCauser, bIsHeadShot);
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

void ACharacterBase::CharacterRevive()
{
	if(HPComponent)
	{
		HPComponent->CharacterRevive();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s] HPComponent is nullptr"), *GetName(), TEXT("CharacterRevive"));
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SetCollisionProfileByTeam(TeamID);
	
	GetMesh()->SetSimulatePhysics(false);

	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -73.5f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	
	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 100.f));
}

void ACharacterBase::SetCollisionProfileByTeam(ETeamID InTeamID)
{
	switch (InTeamID)
	{
	case ETeamID::ETI_Team1:
		GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Team1Capsule")));
		GetMesh()->SetCollisionProfileName(FName(TEXT("Team1Mesh")));
		break;
	case ETeamID::ETI_Team2:
		GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Team2Capsule")));
		GetMesh()->SetCollisionProfileName(FName(TEXT("Team2Mesh")));
		break;
	default:
		break;
	}
}

void ACharacterBase::NotifyCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot)
{
}
