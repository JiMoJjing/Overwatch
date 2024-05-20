#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Utilities.h"

AProjectileBase::AProjectileBase() : HitSphereRadius(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	HitSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphereComponent"));
	SetRootComponent(HitSphereComponent);

	HitSphereComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
	HitSphereComponent->SetSphereRadius(1.f);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(HitSphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	//HitSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnHitSphereBeginOverlap);
	HitSphereComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnSphereHit);
	

	HitSphereComponent->SetSphereRadius(HitSphereRadius);
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
	ProjectileMovementComponent->SetActive(true);
	
	NiagaraComponent->Activate(true);
	
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

	SetActorLocation(FVector::ZeroVector);
	
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->SetActive(false);
	NiagaraComponent->Deactivate();

	
	bCanActivate = true;
}

void AProjectileBase::OnHitSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Deactivate();
}

void AProjectileBase::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Deactivate();
}

