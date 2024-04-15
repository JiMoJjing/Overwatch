#pragma once

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAbilityState : uint8
{
    EAS_None = 0 UMETA(Hidden, DisplayName = "None"),
    EAS_Available = 1 << 0 UMETA(DisplayName = "Available"),
    EAS_Active = 1 << 1 UMETA(DisplayName = "Active"),
    EAS_Cooldown = 1 << 2 UMETA(DisplayName = "Cooldown"),
    EAS_Max = 1 << 7 UMETA(Hidden, DisplayName = "Max")
};
ENUM_CLASS_FLAGS(EAbilityState);

