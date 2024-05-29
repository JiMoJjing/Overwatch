#include "Colliders/Genji/DeflectCollider.h"
#include "Components/BoxComponent.h"
#include "Projectiles/ProjectileBase.h"
#include "Utilities/CLog.h"

ADeflectCollider::ADeflectCollider()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetBoxExtent(FVector(25.f, 50.f, 100.f));
	BoxComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->bHiddenInGame = false;
}

void ADeflectCollider::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeflectCollider::OnBoxBeginOverlap);
}

void ADeflectCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeflectCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AProjectileBase* ProjectileBase = Cast<AProjectileBase>(OtherActor))
	{
		ProjectileBase->Deflected(GetOwner(), GetInstigator(), GetActorForwardVector());
	}
}

