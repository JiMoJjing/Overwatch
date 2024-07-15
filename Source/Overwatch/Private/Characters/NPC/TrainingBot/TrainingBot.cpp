#include "Characters/NPC/TrainingBot/TrainingBot.h"

#include "ActorComponents/Ability/ProjectileAmmoComponent.h"
#include "Components/CapsuleComponent.h"


ATrainingBot::ATrainingBot()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(75.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -73.5f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	
	ProjectileAmmoComponent = CreateDefaultSubobject<UProjectileAmmoComponent>(TEXT("ProjectilePoolComponent"));
}

void ATrainingBot::BeginPlay()
{
	Super::BeginPlay();

	StartFire();
}

void ATrainingBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrainingBot::Fire()
{
	if(ProjectileAmmoComponent)
	{
		ProjectileAmmoComponent->ActivateProjectile(GetMesh()->GetBoneLocation(FName(TEXT("head"))) + FVector(20.f, 0.f, 0.f), GetActorForwardVector());
	}
}

void ATrainingBot::CharacterDeath()
{
	Super::CharacterDeath();
	StopFire();
}

void ATrainingBot::CharacterRevive()
{
	Super::CharacterRevive();
	StartFire();
}

void ATrainingBot::StartFire()
{
	if(bFireMode)
	{
		GetWorldTimerManager().SetTimer(AutoFireTimerHandle, this, &ATrainingBot::Fire, 1.f, true, 2.f);
	}
}

void ATrainingBot::StopFire()
{
	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}

