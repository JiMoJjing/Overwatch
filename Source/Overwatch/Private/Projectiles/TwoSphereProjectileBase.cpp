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

	SetTeamCollisionSettings(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	
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
	
	Deactivate();
	
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
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 28.f, GetOwner()->GetActorLocation(), HitResult, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
}

void ATwoSphereProjectileBase::Deflected(AActor* NewOwner, APawn* NewInstigator, const FVector& Direction)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	SetActorRotation(Direction.Rotation());

	SetTeamCollisionSettings(Cast<ACharacterBase>(NewOwner)->GetTeamID());

	LifeSpanTimerRestart();
	
	ProjectileMovementComponent->Velocity = Direction * ProjectileInitialSpeed;
}

void ATwoSphereProjectileBase::SetTeamCollisionSettings(ETeamID TeamID)
{
	Super::SetTeamCollisionSettings(TeamID);

	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		OverlapSphereCollisionProfileName = FName(TEXT("Team1ProjectileOverlap"));
		TraceTypeQuery = TraceTypeQuery3;
		break;
	case ETeamID::ETI_Team2:
		OverlapSphereCollisionProfileName = FName(TEXT("Team2ProjectileOverlap"));
		TraceTypeQuery = TraceTypeQuery4;
		break;
	default:
		break;
	}
	OverlapSphereComponent->SetCollisionProfileName(OverlapSphereCollisionProfileName);
}
