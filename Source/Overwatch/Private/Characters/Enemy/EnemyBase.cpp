﻿#include "Characters/Enemy/EnemyBase.h"

#include "ActorComponents/Status/HPComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/Enemy/EnemyHPBar.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
	HPBarWidgetComponent->SetupAttachment(RootComponent);
	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Team2Capsule")));
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionProfileName(FName(TEXT("Team2Mesh")));
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	HPBarWidgetComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Collapsed);
	if(UEnemyHPBar* EnemyHPBar = Cast<UEnemyHPBar>(HPBarWidgetComponent->GetUserWidgetObject()))
	{
		GetHPComponent()->OnHPChanged.AddDynamic(EnemyHPBar, &UEnemyHPBar::OnHPChanged);
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HPBarWidgetComponent->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
	
	return DamageAmount;
}