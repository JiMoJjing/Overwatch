#include "Colliders/Genji/SwiftStrikeCollider.h"
#include "Components/BoxComponent.h"

#include "Utilities.h"
#include "Characters/CharacterBase.h"

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

	if(GetOwner())
	{
		SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	}
}

void ASwiftStrikeCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwiftStrikeCollider::Activate() const	
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASwiftStrikeCollider::Deactivate() const
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASwiftStrikeCollider::SetCollisionProfileByTeam(ETeamID TeamID)
{
	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		BoxComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
		break;
	case ETeamID::ETI_Team2:
		BoxComponent->SetCollisionProfileName(FName(TEXT("Team2Collider")));
		break;
	}
}

void ASwiftStrikeCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetOwner())
	{
		UGameplayStatics::ApplyDamage(OtherComp->GetOwner(), 50.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
}

