#include "ActorComponents/Ability/Genji/Genji_DragonbladeComponent.h"

#include "Characters/Player/Genji/Genji.h"
#include "GameFramework/Character.h"

#include "Utilities.h"

UGenji_DragonbladeComponent::UGenji_DragonbladeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UGenji_DragonbladeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGenji_DragonbladeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UGenji_DragonbladeComponent::StartAbility()
{
	DragonbladeDrawingStart();
}

void UGenji_DragonbladeComponent::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	if(bInterrupted == false)
	{
		if(Montage == DragonbladeDrawingMontage)
		{
			DragonbladeDrawingFinish();
		}
		else if(Montage == DragonbladeSheathingMontage)
		{
			DragonbladeSheathingFinish();
		}
	}
}

void UGenji_DragonbladeComponent::DragonbladeDrawingStart()
{
	// Active 상태 켜기 -> 몽타주 재생 -> 게이지 리셋, bActive 켜기
	Super::StartAbility();
	
	if(AGenji* Genji =  Cast<AGenji>(GetOwner()))
	{
		if(DragonbladeDrawingMontage)
		{
			Genji->GetMesh()->GetAnimInstance()->Montage_Play(DragonbladeDrawingMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] DragonbladeDrawingMontage is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("StartAbility"));
		}
		SetUltimateAbilityActive(true);
		ResetGauge();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] Genji is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("StartAbility"));
	}
}

void UGenji_DragonbladeComponent::DragonbladeDrawingFinish()
{
	// Active 상태 끄기 -> Drawing 몽타주 끝났을 때 부터 DragonbladeActive 시작
	if(AGenji* Genji =  Cast<AGenji>(GetOwner()))
	{
		Genji->SetDragonbladeActive(true);
	}
	
	Super::FinishAbility();
	DragonbladeActiveTimerStart();
}

void UGenji_DragonbladeComponent::DragonbladeSheathingStart()
{
	// Active 상태 켜기 -> 몽타주 재생 -> DragonbladeActive 끄기 -> UltimateAbilityActive 끄기
	Super::StartAbility();

	if(AGenji* Genji =  Cast<AGenji>(GetOwner()))
	{
		if(DragonbladeSheathingMontage)
		{
			Genji->GetMesh()->GetAnimInstance()->Montage_Play(DragonbladeSheathingMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] DragonbladeSheathingMontage is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("FinishAbility"));
		}
		Genji->SetDragonbladeActive(false);
		SetUltimateAbilityActive(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] Genji is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("FinishAbility"));
	}
}

void UGenji_DragonbladeComponent::DragonbladeSheathingFinish()
{
	// Active 상태 끄기
	Super::FinishAbility();
}

void UGenji_DragonbladeComponent::DragonbladeActiveTimerStart()
{
	if(GetOwner())
	{
		DragonbladeActiveCurrentTime = DragonbladeActiveTime;
		GetOwner()->GetWorldTimerManager().SetTimer(DragonbladeActiveTimerHandle, this, &UGenji_DragonbladeComponent::DragonbladeActiveTimerTick, 0.1f, true);
	}
}

void UGenji_DragonbladeComponent::DragonbladeActiveTimerTick()
{
	DragonbladeActiveCurrentTime -= 0.1f;
	CLog::Print(DragonbladeActiveCurrentTime, 10, 0.1f, FColor::Red);
	if (FMath::IsNearlyZero(DragonbladeActiveCurrentTime, 0.01f))
	{
		DragonbladeActiveTimerEnd();
	}
}

void UGenji_DragonbladeComponent::DragonbladeActiveTimerEnd()
{
	if(GetOwner()->GetWorldTimerManager().IsTimerActive(DragonbladeActiveTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(DragonbladeActiveTimerHandle);
	}
	
	DragonbladeSheathingStart();
}

