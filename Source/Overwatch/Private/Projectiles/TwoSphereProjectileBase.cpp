#include "Projectiles/TwoSphereProjectileBase.h"

#include "Characters/CharacterBase.h"
#include "Colliders/Genji/DeflectCollider.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CLog.h"

ATwoSphereProjectileBase::ATwoSphereProjectileBase()
{
	OverlapSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("OverlapSphereComponent"));
	OverlapSphereComponent->SetupAttachment(HitSphereComponent);
	OverlapSphereComponent->SetGenerateOverlapEvents(true);
	OverlapSphereComponent->SetSphereRadius(10.f);
	OverlapSphereComponent->CanCharacterStepUpOn = ECB_No;
}

void ATwoSphereProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	}
	
	OverlapSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATwoSphereProjectileBase::ATwoSphereProjectileBase::OnOverlapSphereBeginOverlap);
	OverlapSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlapSphereComponent->SetSphereRadius(OverlapSphereRadius);
	OverlapSphereComponent->SetActive(false);
}

void ATwoSphereProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwoSphereProjectileBase::Activate(const FVector& StartLocation, const FVector& Direction)
{
	Super::Activate(StartLocation, Direction);
	OverlapSphereComponent->SetActive(true);
	OverlapSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATwoSphereProjectileBase::Deactivate()
{
	Super::Deactivate();
	OverlapSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlapSphereComponent->SetActive(false);
}

void ATwoSphereProjectileBase::OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ADeflectCollider* DeflectCollider = Cast<ADeflectCollider>(OtherActor))
	{
		Deflected(DeflectCollider->GetInstigator(), DeflectCollider->GetDeflectDirection(GetActorLocation()));
		return;
	}
	
	if(OtherActor || OtherComp)
	{
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		FVector SweepLocation = GetActorLocation();
		FCollisionQueryParams Params(NAME_None, false);
		
		if(GetWorld()->SweepSingleByChannel(
			HitResult,
			SweepLocation,
			SweepLocation,
			FQuat::Identity,
			TraceChannel,
			FCollisionShape::MakeSphere(OverlapSphereRadius),
			Params))
		{
			if (HitResult.GetActor())
			{
				UGameplayStatics::ApplyPointDamage(
					HitResult.GetActor(),
					Damage,
					GetInstigator()->GetActorLocation(),
					HitResult,
					GetInstigator()->GetController(),
					this, UDamageType::StaticClass());
			}
			SpawnHitActorEffect(HitResult.Location, FRotator::ZeroRotator);
		}
	}

	Deactivate();
}

void ATwoSphereProjectileBase::SetCollisionProfileByTeam(ETeamID TeamID)
{
	Super::SetCollisionProfileByTeam(TeamID);

	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		OverlapSphereComponent->SetCollisionProfileName(FName(TEXT("Team1ProjectileOverlap")));
		TraceChannel = ECC_GameTraceChannel9;
		break;
	case ETeamID::ETI_Team2:
		OverlapSphereComponent->SetCollisionProfileName(FName(TEXT("Team2ProjectileOverlap")));
		TraceChannel = ECC_GameTraceChannel10;
		break;
	default:
		break;
	}
}

void ATwoSphereProjectileBase::Deflected(APawn* NewInstigator, const FVector& Direction)
{
	SetActorRotation(Direction.Rotation());
	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
	
	SetInstigator(NewInstigator);
	if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetInstigator()))
	{
		SetCollisionProfileByTeam(CharacterBase->GetTeamID());
	}
}
