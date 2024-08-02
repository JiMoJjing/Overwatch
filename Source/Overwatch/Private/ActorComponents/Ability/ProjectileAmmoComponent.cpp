#include "ActorComponents/Ability/ProjectileAmmoComponent.h"

#include "Projectiles/ProjectileBase.h"
#include "Utilities.h"


UProjectileAmmoComponent::UProjectileAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectileAmmoComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializePool();
}

void UProjectileAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UProjectileAmmoComponent::ActivateProjectile(const FVector& StartLocation, const FVector& Direction)
{
	if(ProjectilePool.IsEmpty())
	{
		if(AProjectileBase* ProjectileBase = Expand())
		{
			ProjectileBase->Activate(StartLocation, Direction);
		}
	}
	else
	{
		AProjectileBase* ProjectileBase = nullptr;
		if(ProjectilePool.Dequeue(ProjectileBase))
		{
			ProjectileBase->Activate(StartLocation, Direction);
		}
	}
}

void UProjectileAmmoComponent::DeactivateProjectile(AProjectileBase* ProjectileBase)
{
	if(ProjectileBase)
	{
		ProjectilePool.Enqueue(ProjectileBase);
	}
}

void UProjectileAmmoComponent::InitializePool()
{
	if(ProjectileClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FVector SpawnLocation = FVector::ZeroVector + FVector(0.f, 0.f, -200.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		for (int i = 0; i < PoolSize; i++)
		{
			if (AProjectileBase* ProjectileObject = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams))
			{
				ProjectileObject->SetProjectileAmmoComponent(this);
				ProjectilePool.Enqueue(ProjectileObject);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] ProjectileClass is nullptr"),*GetOwner()->GetName(), *GetName(), TEXT("InitializePool"));
	}
}

AProjectileBase* UProjectileAmmoComponent::Expand()
{
	if(ProjectileClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		if (AProjectileBase* ProjectileBase = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams))
		{
			ProjectileBase->SetProjectileAmmoComponent(this);
			return ProjectileBase;
		}
	}
	return nullptr;
}

