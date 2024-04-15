#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Utilities.h"

AProjectileBase::AProjectileBase() : ProjectileSphereRadius(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(ProjectileSphereComponent);

	ProjectileSphereComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
	ProjectileSphereComponent->SetSphereRadius(1.f);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereBeginOverlap);
	ProjectileSphereComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnSphereHit);
	

	ProjectileSphereComponent->SetSphereRadius(ProjectileSphereRadius);
	ProjectileSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = ProjectileGravityScale;
	ProjectileMovementComponent->SetActive(false);
	
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::Activate(const FVector& StartLocation, const FVector& Direction)
{
	SetActorLocation(StartLocation);
	SetActorRotation(Direction.Rotation());
	
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	ProjectileSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMovementComponent->SetActive(true);
	
	if (LifeSpan != 0.0f)
	{
		GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::Deactivate, LifeSpan, false);
	}
	
	bCanActivate = false;
}

void AProjectileBase::Deactivate()
{
	if (GetWorldTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifeSpanTimerHandle);
	}

	ProjectileSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->SetActive(false);

	SetActorLocation(FVector::ZeroVector);
	
	bCanActivate = true;
}

void AProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Deactivate();
}

void AProjectileBase::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Deactivate();
}

