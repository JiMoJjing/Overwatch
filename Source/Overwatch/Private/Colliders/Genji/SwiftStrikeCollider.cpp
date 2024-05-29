#include "Colliders/Genji/SwiftStrikeCollider.h"
#include "Components/BoxComponent.h"

#include "Utilities.h"

ASwiftStrikeCollider::ASwiftStrikeCollider()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetBoxExtent(FVector(64.f, 64.f, 64.f));
	BoxComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASwiftStrikeCollider::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASwiftStrikeCollider::OnBoxBeginOverlap);
}

void ASwiftStrikeCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwiftStrikeCollider::SwiftStrikeStarted() const	
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASwiftStrikeCollider::SwiftStrikeFinished() const
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASwiftStrikeCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetOwner())
	{
		UGameplayStatics::ApplyDamage(OtherComp->GetOwner(), 50.f, GetInstigator()->GetController(), GetOwner(), UDamageType::StaticClass());
	}
}

