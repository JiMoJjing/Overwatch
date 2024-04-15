#include "Colliders/SwiftStrikeCollider.h"
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

void ASwiftStrikeCollider::SwiftStrikeStarted()
{
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASwiftStrikeCollider::SwiftStrikeFinished()
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASwiftStrikeCollider::SetCollisionEnabled(ECollisionEnabled::Type InNewType)
{
	BoxComponent->SetCollisionEnabled(InNewType);
}

void ASwiftStrikeCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp)
	{
		FString str = FString::Printf(TEXT("OverlappedActor : %s"), *OtherComp->GetOwner()->GetName());
		CLog::Print(str);
	}
}

