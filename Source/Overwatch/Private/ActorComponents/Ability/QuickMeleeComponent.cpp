#include "ActorComponents/Ability/QuickMeleeComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Characters/Player/PlayerBase.h"
#include "Kismet/GameplayStatics.h"

UQuickMeleeComponent::UQuickMeleeComponent()
{

}

void UQuickMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQuickMeleeComponent::StartAbility()
{
	Super::StartAbility();
	PlayAbilityMontage();
	QuickMeleeSweep();
	CooldownTimerStart();
}

void UQuickMeleeComponent::CooldownTimerEnd()
{
	Super::CooldownTimerEnd();
	FinishAbility();
}

void UQuickMeleeComponent::PlayAbilityMontage()
{
	if (APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		PlayerBase->GetMesh()->GetAnimInstance()->Montage_Play(AbilityMontage, 3.f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] PlayerBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("PlayAbilityMontage"));
	}
}

void UQuickMeleeComponent::QuickMeleeSweep()
{
	if(AActor* Owner = GetOwner())
	{
		AController* OwnerController = Cast<ACharacter>(Owner)->GetController();
		FVector ForwardVector = OwnerController->GetControlRotation().Vector();
		FVector OwnerLocation = Owner->GetActorLocation();
		FVector SweepLocation = OwnerLocation + ForwardVector * 125.f;
		TArray<AActor*> IgnoreActors;
		TArray<FHitResult> HitResults;
		TArray<AActor*> ActorsToDamage;

#if WITH_EDITOR
		DrawDebugBox(GetWorld(), SweepLocation, QuickMeleeSweepBoxExtend, ForwardVector.ToOrientationQuat(),  FColor::Green, false, 2.f);
#endif
		GetWorld()->SweepMultiByChannel(HitResults, SweepLocation, SweepLocation, ForwardVector.ToOrientationQuat(), ECC_GameTraceChannel9, FCollisionShape::MakeBox(QuickMeleeSweepBoxExtend));
		
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
				UGameplayStatics::ApplyDamage(Actor, QuickMeleeDamage, OwnerController, Owner, UDamageType::StaticClass());
				SpawnQuickMeleeHitEffect(Actor->GetActorLocation(), FRotator::ZeroRotator);
			}
		}
	}
}

void UQuickMeleeComponent::SpawnQuickMeleeHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	if(QuickMeleeHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), QuickMeleeHitEffect, SpawnLocation, SpawnRotation);
	}
}
