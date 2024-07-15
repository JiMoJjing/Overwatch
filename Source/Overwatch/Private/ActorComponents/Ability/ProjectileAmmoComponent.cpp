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
		if(AProjectileBase* ProjectileBase = ProjectilePool.Top())
		{
			ProjectileBase->Activate(StartLocation, Direction);
			ProjectilePool.Pop();
		}
	}
}

void UProjectileAmmoComponent::DeactivateProjectile(AProjectileBase* ProjectileBase)
{
	if(ProjectileBase)
	{
		ProjectilePool.Push(ProjectileBase);
	}
}

void UProjectileAmmoComponent::InitializePool()
{
	if(ProjectileClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		for (int i = 0; i < PoolSize; i++)
		{
			if (AProjectileBase* ProjectileObject = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams))
			{
				ProjectileObject->SetProjectileAmmoComponent(this);
				ProjectilePool.Add(ProjectileObject);
			}
		}
	}
	else
	{
		CLog::Print("UProjectileAmmoComponent BeginPlay ProjectileClass nullptr");
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

