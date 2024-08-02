#include "Colliders/Genji/DeflectCollider.h"

#include "NiagaraComponent.h"
#include "ActorComponents/Ability/Genji/Genji_DeflectComponent.h"
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

	BoxComponent->SetCollisionProfileName(TEXT("Team1Collider"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->CanCharacterStepUpOn = ECB_No;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DeflectEffect"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetRelativeLocation(FVector(-60.f, 0.f, 0.f));
	NiagaraComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->SetActive(false);
}

void ADeflectCollider::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeflectCollider::OnBoxBeginOverlap);
	BoxComponent->SetBoxExtent(BoxExtend);
	BoxComponent->SetRelativeLocation(RelativeLocation);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetActive(false);

	if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwner()))
	{
		SetCollisionProfileByTeam(CharacterBase->GetTeamID());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s] CharacterBase is nullptr"), *GetName(), TEXT("BeginPlay"));
	}
}

void ADeflectCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeflectCollider::SetGenji_DeflectComponent(UGenji_DeflectComponent* Genji_DeflectComponent)
{
	if(Genji_DeflectComponent)
	{
		DeflectComponent = Genji_DeflectComponent;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s] Genji_DeflectComponent is nullptr"), *GetName(), TEXT("SetGenji_DeflectComponent"));
	}
}

void ADeflectCollider::Activate() const
{
	BoxComponent->SetActive(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	NiagaraComponent->SetActive(true);
	NiagaraComponent->SetVisibility(true);
}

void ADeflectCollider::Deactivate() const
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetActive(false);
	
	NiagaraComponent->SetVisibility(false);
	NiagaraComponent->SetActive(false);
}

FVector ADeflectCollider::GetDeflectDirection(const FVector& StartLocation) const
{
	PlayDeflectMontage();
	FVector OutDirection;
	if(APlayerBase* PlayerBase = Cast<APlayerBase>(GetOwner()))
	{
		PlayerBase->GetDirectionToCrosshair(StartLocation, OutDirection, ECC_Visibility);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] PlayerBase is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("GetDeflectDirection"));
	}
	return OutDirection;
}

void ADeflectCollider::SetCollisionProfileByTeam(ETeamID TeamID)
{
	switch (TeamID)
	{
	case ETeamID::ETI_Team1:
		BoxComponent->SetCollisionProfileName(FName(TEXT("Team1Collider")));
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);
		break;
	case ETeamID::ETI_Team2:
		BoxComponent->SetCollisionProfileName(FName(TEXT("Team2Collider")));
		BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
		break;
	}
}

void ADeflectCollider::PlayDeflectMontage() const
{
	if(DeflectComponent.IsValid())
	{
		DeflectComponent->PlayAbilityMontage();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s -> %s -> %s] DeflectComponent is nullptr"), *GetOwner()->GetName(), *GetName(), TEXT("PlayDeflectMontage"));
	}
}

void ADeflectCollider::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

