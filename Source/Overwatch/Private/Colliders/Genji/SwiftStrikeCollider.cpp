#include "Colliders/Genji/SwiftStrikeCollider.h"

#include "NiagaraFunctionLibrary.h"
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

	if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwner()))
	{
		SetCollisionProfileByTeam(CharacterBase->GetTeamID());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] CharacterBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("BeginPlay"));
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
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Ignore);
		break;
	case ETeamID::ETI_Team2:
		BoxComponent->SetCollisionProfileName(FName(TEXT("Team2Collider")));
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Ignore);
		break;
	}
}

void ASwiftStrikeCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AActor* HitActor = OtherComp->GetOwner())
	{
		UGameplayStatics::ApplyDamage(HitActor, 50.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		SpawnSwiftStrikeHitEffect(HitActor->GetActorLocation(), FRotator::ZeroRotator);
	}
}

void ASwiftStrikeCollider::SpawnSwiftStrikeHitEffect(const FVector& SpawnLocation, const FRotator& SpawnRotation) const
{
	if(SwiftStrikeHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SwiftStrikeHitEffect, SpawnLocation, SpawnRotation);
	}
}

