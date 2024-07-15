#include "Colliders/Genji/DeflectCollider.h"

#include "NiagaraComponent.h"
#include "Characters/CharacterBase.h"
#include "Characters/Player/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Projectiles/ProjectileBase.h"

#include "Utilities/CLog.h"

ADeflectCollider::ADeflectCollider()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetBoxExtent(FVector(25.f, 50.f, 100.f));
	BoxComponent->SetCollisionProfileName(TEXT("Team1Collider"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->CanCharacterStepUpOn = ECB_No;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DeflectEffect"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetRelativeLocation(FVector(-60.f, 0.f, 0.f));
	NiagaraComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->Deactivate();
}

void ADeflectCollider::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->SetRelativeLocation(FVector(60.f, 0.f, 0.f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeflectCollider::OnBoxBeginOverlap);

	if(GetOwner())
	{
		SetCollisionProfileByTeam(Cast<ACharacterBase>(GetOwner())->GetTeamID());
	}
}

void ADeflectCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeflectCollider::Activate() const
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	NiagaraComponent->Activate(true);
	NiagaraComponent->SetVisibility(true);
}

void ADeflectCollider::Deactivate() const
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->Deactivate();
}

void ADeflectCollider::SetCollisionProfileByTeam(ETeamID TeamID)
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

void ADeflectCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AProjectileBase* ProjectileBase = Cast<AProjectileBase>(OtherActor))
	{
		FVector OutDirection;
		Cast<APlayerBase>(GetOwner())->GetDirectionToCrosshair(ProjectileBase->GetActorLocation(), OutDirection, ECC_Visibility);
		ProjectileBase->Deflected(GetInstigator(), OutDirection);
	}
}

