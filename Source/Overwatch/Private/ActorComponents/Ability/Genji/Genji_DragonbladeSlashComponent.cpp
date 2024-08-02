#include "ActorComponents/Ability/Genji/Genji_DragonbladeSlashComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Characters/Player/PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


UGenji_DragonbladeSlashComponent::UGenji_DragonbladeSlashComponent()
{
	
}

void UGenji_DragonbladeSlashComponent::BeginPlay()
{
	Super::BeginPlay();
	SetCollisionProfileByTeam();
}

void UGenji_DragonbladeSlashComponent::StartAbility()
{
	Super::StartAbility();
	PlayAbilityMontage();
	CooldownTimerStart();
}

void UGenji_DragonbladeSlashComponent::CooldownTimerEnd()
{
	Super::CooldownTimerEnd();
	FinishAbility();
}

void UGenji_DragonbladeSlashComponent::PlayAbilityMontage()
{
	if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		if(AbilityMontage)
		{
			FName SectionName;
			switch (DragonbladeSlashMontageSectionCount)
			{
			case 0:
				SectionName = FName(TEXT("Section_01"));
				break;
			case 1:
				SectionName = FName(TEXT("Section_02"));
				break;
			default:
				break;
			}
			PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage);
			PlayerBase->GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, AbilityMontage);

			++DragonbladeSlashMontageSectionCount;
			DragonbladeSlashMontageSectionCount %= 2;

			DragonbladeSlashSweep();
		}
	}
}

void UGenji_DragonbladeSlashComponent::DragonbladeSlashSweep()
{
	if(AActor* Owner = GetOwner())
	{
		AController* OwnerController = Cast<ACharacter>(Owner)->GetController();
		FVector ForwardVector = OwnerController->GetControlRotation().Vector();
		FVector OwnerLocation = Owner->GetActorLocation();
		FVector SweepLocation = OwnerLocation + ForwardVector * 150.f;
		TArray<AActor*> IgnoreActors;
		TArray<FHitResult> HitResults;
		TArray<AActor*> ActorsToDamage;

#if WITH_EDITOR
		DrawDebugBox(GetWorld(), SweepLocation, SlashSweepBoxExtend, ForwardVector.ToOrientationQuat(),  FColor::Green, false, 2.f);
#endif
		GetWorld()->SweepMultiByChannel(HitResults, SweepLocation, SweepLocation, ForwardVector.ToOrientationQuat(), ECC_GameTraceChannel9, FCollisionShape::MakeBox(SlashSweepBoxExtend));
		
		for(auto& HitResult : HitResults)
		{
			if(AActor* HitActor = HitResult.GetComponent()->GetOwner())
			{
				ActorsToDamage.AddUnique(HitActor);
			}
		}
		for(AActor* Actor : ActorsToDamage)
		{
			FHitResult BlockHitResult;
			FVector HitActorLocation = Actor->GetActorLocation();
			FCollisionQueryParams Params(NAME_None, false);
			if(GetWorld()->LineTraceSingleByChannel(BlockHitResult, OwnerLocation, HitActorLocation, ECC_Camera, Params) == false)
			{
#if WITH_EDITOR
				DrawDebugLine(GetWorld(), OwnerLocation, HitActorLocation, FColor::Green, false, 2.f);
#endif
				UGameplayStatics::ApplyDamage(Actor, DragonbladeSlashDamage, OwnerController, Owner, UDamageType::StaticClass());
				SpawnDragonbladeHitEffect(Actor->GetActorLocation(), FRotator::ZeroRotator);
			}
		}
	}
}

void UGenji_DragonbladeSlashComponent::SpawnDragonbladeHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	if(DragonbladeSlashHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DragonbladeSlashHitEffect, SpawnLocation, SpawnRotation);
	}
}

void UGenji_DragonbladeSlashComponent::SetCollisionProfileByTeam()
{
	if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwner()))
	{
		switch (CharacterBase->GetTeamID())
		{
		case ETeamID::ETI_Team1:
			SlashSweepTraceChannel = ECC_GameTraceChannel9;
			break;
		case ETeamID::ETI_Team2:
			SlashSweepTraceChannel = ECC_GameTraceChannel10;
			break;
		default:
			break;
		}
	}
}
