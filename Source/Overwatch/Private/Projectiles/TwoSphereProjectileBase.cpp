#include "Projectiles/TwoSphereProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CLog.h"

ATwoSphereProjectileBase::ATwoSphereProjectileBase()
{
	OverlapSphereComponent = CreateDefaultSubobject<USphereComponent>(FName("OverlapSphereComponent"));
	OverlapSphereComponent->SetupAttachment(HitSphereComponent);
	OverlapSphereComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
}

void ATwoSphereProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	OverlapSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATwoSphereProjectileBase::ATwoSphereProjectileBase::OnOverlapSphereBeginOverlap);
	OverlapSphereComponent->SetSphereRadius(OverlapSphereRadius);
}

void ATwoSphereProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwoSphereProjectileBase::OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	UKismetSystemLibrary::SphereTraceSingle(this, GetActorLocation(), GetActorLocation() + FVector(0.f, 0.f, -0.1f), OverlapSphereRadius, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	if(HitResult.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 28.f, GetOwner()->GetActorLocation(), HitResult, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
	Deactivate();
}
