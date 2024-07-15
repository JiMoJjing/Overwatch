#include "Characters/NPC/NPCBase.h"

#include "ActorComponents/Status/HPComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponents/NPC/NPCWidgetComponent.h"
#include "Widgets/NPC/NPCWidget.h"
#include "Utilities.h"
#include "Controllers/OverwatchNPCAIController.h"

ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = true;

	NPCWidgetComponent = CreateDefaultSubobject<UNPCWidgetComponent>(TEXT("HPBarWidgetComponent"));
	NPCWidgetComponent->SetupAttachment(GetMesh());
	NPCWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	NPCWidgetComponent->SetDrawSize(FVector2D(100.f, 100.f));
	
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Team1Capsule")));
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;
	
	GetMesh()->SetCollisionProfileName(FName(TEXT("Team1Mesh")));
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

	if(UUserWidget* UserWidget = NPCWidgetComponent->GetUserWidgetObject())
	{
		if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(UserWidget->GetOwningPlayerPawn()))
		{
			if(TeamID == CharacterBase->GetTeamID())
			{
				SetNPCWidgetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				SetNPCWidgetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	SetCollisionProfileByTeam(TeamID);
}

void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ANPCBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	SetNPCWidgetVisibility(ESlateVisibility::Visible);
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	
	return DamageAmount;
}

void ANPCBase::CharacterDeath()
{
	SetNPCWidgetVisibility(ESlateVisibility::Collapsed);
	
	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	GetMesh()->SetCollisionProfileName(FName(TEXT("Ragdoll")));
	GetMesh()->SetSimulatePhysics(true);
	
	FTimerHandle ReviveTimerHandle;
	GetWorldTimerManager().SetTimer(ReviveTimerHandle, this, &ACharacterBase::CharacterRevive, 5.f, false);
}

void ANPCBase::NotifyCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot)
{
	if(AOverwatchNPCAIController* OverwatchNPCAIController = Cast<AOverwatchNPCAIController>(GetController()))
	{
		CLog::Print(TEXT("Death!"));
		OverwatchNPCAIController->ReceiveCharacterDeath(EventInstigator, DamageCauser, bIsHeadShot);
	}
}

void ANPCBase::CharacterRevive()
{
	Super::CharacterRevive();
	
	if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		if(TeamID == CharacterBase->GetTeamID())
		{
			SetNPCWidgetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ANPCBase::SetNPCWidgetVisibility(ESlateVisibility InVisibility)
{
	if(UUserWidget* UserWidget = NPCWidgetComponent->GetUserWidgetObject())
	{
		UserWidget->SetVisibility(InVisibility);
	}
}
