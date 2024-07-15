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
	OverlapSphereComponent->SetupAttachment(RootComponent);
}

void ATwoSphereProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner())
	{
		SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	}
	
	OverlapSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATwoSphereProjectileBase::ATwoSphereProjectileBase::OnOverlapSphereBeginOverlap);
	OverlapSphereComponent->SetSphereRadius(OverlapSphereRadius);
}

void ATwoSphereProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwoSphereProjectileBase::OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->IsA(ADeflectCollider::StaticClass()))
	{
		return;
	}
	
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	UKismetSystemLibrary::SphereTraceSingle
	(
		this,
		GetActorLocation(),
		GetActorLocation() + FVector(0.f, 0.f, -0.1f),
		OverlapSphereRadius,
		TraceTypeQuery, //1은 Visibility, 2는 Camera, 3부터 커스텀 
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);
	
	if(HitResult.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), Damage, GetInstigator()->GetActorLocation(), HitResult, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}

	Deactivate();
}

void ATwoSphereProjectileBase::Deflected(APawn* NewInstigator, const FVector& Direction)
{
	SetInstigator(NewInstigator);
	SetActorRotation(Direction.Rotation());

	SetCollisionProfileByTeam(Cast<ACharacterBase>(NewInstigator)->GetTeamID());

	LifeSpanTimerRestart();
	
	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
}

void ATwoSphereProjectileBase::SetCollisionProfileByTeam(ETeamID TeamID)
{
	Super::SetCollisionProfileByTeam(TeamID);

	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		OverlapSphereComponent->SetCollisionProfileName(FName(TEXT("Team1ProjectileOverlap")));
		TraceTypeQuery = TraceTypeQuery3;
		break;
	case ETeamID::ETI_Team2:
		OverlapSphereComponent->SetCollisionProfileName(FName(TEXT("Team2ProjectileOverlap")));
		TraceTypeQuery = TraceTypeQuery4;
		break;
	default:
		break;
	}
}

void ATwoSphereProjectileBase::Activate(const FVector& StartLocation, const FVector& Direction)
{
	Super::Activate(StartLocation, Direction);
	OverlapSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATwoSphereProjectileBase::Deactivate()
{
	Super::Deactivate();
	OverlapSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
