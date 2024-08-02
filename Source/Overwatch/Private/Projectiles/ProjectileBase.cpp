#include "Projectiles/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Utilities.h"
#include "ActorComponents/Ability/ProjectileAmmoComponent.h"
#include "Characters/CharacterBase.h"

AProjectileBase::AProjectileBase() : HitSphereRadius(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	HitSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphereComponent"));
	SetRootComponent(HitSphereComponent);

	HitSphereComponent->CanCharacterStepUpOn = ECB_No;
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
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitSphereComponent->SetSphereRadius(HitSphereRadius);
	HitSphereComponent->SetActive(false);

	ProjectileMovementComponent->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileMaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = ProjectileGravityScale;
	ProjectileMovementComponent->SetActive(false);

	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->SetActive(false);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::SetProjectileAmmoComponent(UProjectileAmmoComponent* InComponent)
{
	if(InComponent)
	{
		ProjectileAmmoComponent = InComponent;
	}
}

void AProjectileBase::Activate(const FVector& StartLocation, const FVector& Direction)
{
	SetActorLocation(StartLocation);
	SetActorRotation(Direction.Rotation());

	HitSphereComponent->SetActive(true);
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	NiagaraComponent->SetActive(true);
	NiagaraComponent->SetVisibility(true);
	
	ProjectileMovementComponent->SetActive(true);
	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
	
	LifeSpanTimerStart();
}

void AProjectileBase::Deactivate()
{
	SetActorLocation(FVector::ZeroVector);
	LifeSpanTimerEnd();
	
	HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitSphereComponent->SetActive(false);
	
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	ProjectileMovementComponent->SetActive(false);
	
	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->SetActive(false);
	
	if(Cast<APawn>(GetOwner()) != GetInstigator())
	{
		SetInstigator(Cast<APawn>(GetOwner()));
	}
	SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());

	if(ProjectileAmmoComponent.IsValid())
	{
		ProjectileAmmoComponent->DeactivateProjectile(this);
	}
}


void AProjectileBase::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SpawnHitWallEffect(Hit.Location, FRotator::ZeroRotator);
	Deactivate();
}

void AProjectileBase::LifeSpanTimerStart()
{
	if (LifeSpan != 0.0f)
	{
		GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::Deactivate, LifeSpan, false);
	}
}

void AProjectileBase::LifeSpanTimerEnd()
{
	if (GetWorldTimerManager().IsTimerActive(LifeSpanTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifeSpanTimerHandle);
	}
}

void AProjectileBase::LifeSpanTimerRestart()
{
	LifeSpanTimerEnd();
	LifeSpanTimerStart();
}

void AProjectileBase::SpawnHitActorEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	if(HitActorEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitActorEffect, SpawnLocation, SpawnRotation);
	}
}

void AProjectileBase::SpawnHitWallEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	if(HitWallEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitWallEffect, SpawnLocation, SpawnRotation);
	}
}

void AProjectileBase::SetCollisionProfileByTeam(ETeamID TeamID)
{
	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		HitSphereComponent->SetCollisionProfileName(FName(TEXT("Team1ProjectileHit")));
		break;
	case ETeamID::ETI_Team2:
		HitSphereComponent->SetCollisionProfileName(FName(TEXT("Team2ProjectileHit")));
		break;
	default:
		break;
	}
}

void AProjectileBase::Deflected(APawn* NewInstigator, const FVector& Direction)
{
	SetActorRotation(Direction.Rotation());
	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
	
	SetInstigator(NewInstigator);
	if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetInstigator()))
	{
		SetCollisionProfileByTeam(CharacterBase->GetTeamID());
	}
}

