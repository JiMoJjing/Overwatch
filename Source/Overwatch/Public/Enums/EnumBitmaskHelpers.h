#pragma once

#include "CoreMinimal.h"
#include "AbilityState.h"

class OVERWATCH_API CEnumBitmaskHelpers
{
public:
	static inline void AddAbilityState(uint8& InNowAbilityState, EAbilityState InAbilityState);
	static inline void SubAbilityState(uint8& InNowAbilityState, EAbilityState InAbilityState);
	static inline bool IsAbilityState(const uint8 InNowAbilityState, EAbilityState InAbilityState);
};