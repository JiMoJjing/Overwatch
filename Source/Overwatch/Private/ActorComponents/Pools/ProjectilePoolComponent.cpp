#include "ActorComponents/Pools/ProjectilePoolComponent.h"

#include "Projectiles/ProjectileBase.h"
#include "Utilities.h"

UProjectilePoolComponent::UProjectilePoolComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectilePoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PooledProjectileClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FVector SpawnLocation = FVector::ZeroVector;
		FRotator SpawnRotation = FRotator::ZeroRotator;

		for (int i = 0; i < PoolSize; i++)
		{
			AProjectileBase* ProjectileObject = GetWorld()->SpawnActor<AProjectileBase>(PooledProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
			if (ProjectileObject)
			{
				ProjectilePools.Add(ProjectileObject);
			}
		}
	}
	else
	{
		CLog::Log(TEXT("UProjectilePoolComponent BeginPlay PooledProjectileClass nullptr"));
	}
}

void UProjectilePoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UProjectilePoolComponent::ActivateProjectile(const FVector& StartLocation, const FVector& Direction)
{
	AProjectileBase* ProjectileObject = ProjectilePools[PoolIndex];
	CLog::Print(PoolIndex);
	if (ProjectileObject && ProjectileObject->CanActivate())
	{
		ProjectileObject->Activate(StartLocation, Direction);
		PoolIndex++;
	}
	else
	{
		// 무한 루프 빠질 가능성 있으므로 개선할 것.
		PoolIndex++;
		ActivateProjectile(StartLocation, Direction);
	}
	PoolIndex %= PoolSize;
}