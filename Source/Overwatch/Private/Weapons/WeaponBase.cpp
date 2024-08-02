#include "Weapons/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("WeaponMesh")));
	SetRootComponent(WeaponMesh);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}