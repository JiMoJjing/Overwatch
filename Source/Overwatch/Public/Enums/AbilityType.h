#pragma once

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAbilityType : uint8
{
    EAT_None = 0 UMETA(Hidden, DisplayName = "None"),
    EAT_PrimaryFire = 1 << 0 UMETA(DisplayName = "PrimaryFire"),
	EAT_SecondaryFire = 1 << 1 UMETA(DisplayName = "SecondaryFire"),
    EAT_AbilityOne = 1 << 2 UMETA(DisplayName = "AbilityOne"),
    EAT_AbilityTwo = 1 << 3 UMETA(DisplayName = "AbilityTwo"),
    EAT_AbilityThree = 1 << 4 UMETA(DisplayName = "AbilityThree"),
    EAT_QuickMelee = 1 << 5 UMETA(DisplayName = "QuickMelee"),
    EAT_Reloading = 1 << 6 UMETA(DisplayName = "Reloading"),
    EAT_Max = 1 << 7 UMETA(Hidden, DisplayName = "Max")
};
ENUM_CLASS_FLAGS(EAbilityType);