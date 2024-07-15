#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Utilities.h"
#include "ActorComponents/Ability/ProjectileAmmoComponent.h"
#include "Characters/CharacterBase.h"

AProjectileBase::AProjectileBase() : HitSphereRadius(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	HitSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphereComponent"));
	SetRootComponent(HitSphereComponent);
	
	HitSphereComponent->SetGenerateOverlapEvents(false);
	HitSphereComponent->SetSphereRadius(1.f);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(HitSphereComponent);
	NiagaraComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	}
	
	HitSphereComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnSphereHit);
	HitSphereComponent->SetSphereRadius(HitSphereRadius);
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = ProjectileGravityScale;
	ProjectileMovementComponent->SetActive(false);

	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->Deactivate();
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

	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
	ProjectileMovementComponent->SetActive(true);
	
	NiagaraComponent->Activate(true);
	NiagaraComponent->SetVisibility(true);
	
	if (LifeSpan != 0.0f)
	{
		GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::Deactivate, LifeSpan, false);
	}
}

void AProjectileBase::Deactivate()
{
	if (GetWorldTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifeSpanTimerHandle);
	}
	if(Cast<APawn>(GetOwner()) != GetInstigator())
	{
		SetInstigator(Cast<APawn>(GetOwner()));
	}
	SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->SetActive(false);
	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->Deactivate();

	if(ProjectileAmmoComponent.IsValid())
	{
		ProjectileAmmoComponent->DeactivateProjectile(this);
	}
}

void AProjectileBase::OnHitSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Deactivate();
}

void AProjectileBase::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Deactivate();
}

void AProjectileBase::Deflected(APawn* NewInstigator, const FVector& Direction)
{
	SetInstigator(NewInstigator);
	SetActorRotation(Direction.Rotation());
	
	SetCollisionProfileByTeam(Cast<ACharacterBase>(NewInstigator)->GetTeamID());
	
	LifeSpanTimerRestart();
	
	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
}

void AProjectileBase::SetProjectileAmmoComponent(UProjectileAmmoComponent* InComponent)
{
	if(InComponent)
	{
		ProjectileAmmoComponent = InComponent;
	}
}

void AProjectileBase::LifeSpanTimerRestart()
{
	if (GetWorldTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifeSpanTimerHandle);
	}
	
	if (LifeSpan != 0.0f)
	{
		GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::Deactivate, LifeSpan, false);
	}
}

void AProjectileBase::SetCollisionProfileByTeam(ETeamID TeamID)
{
	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		HitSphereCollisionProfileName = FName(TEXT("Team1ProjectileHit"));
		break;
	case ETeamID::ETI_Team2:
		HitSphereCollisionProfileName = FName(TEXT("Team2ProjectileHit"));
		break;
	default:
		break;
	}
	
	HitSphereComponent->SetCollisionProfileName(HitSphereCollisionProfileName);
}

