#include "EnumBitmaskHelpers.h"

inline void CEnumBitmaskHelpers::AddAbilityState(uint8& InNowAbilityState, EAbilityState InAbilityState)
{
	InNowAbilityState |= static_cast<uint8>(InAbilityState);
}

inline void CEnumBitmaskHelpers::SubAbilityState(uint8& InNowAbilityState, EAbilityState InAbilityState)
{
	InNowAbilityState &= ~static_cast<uint8>(InAbilityState);
}

inline bool CEnumBitmaskHelpers::IsAbilityState(const uint8 InNowAbilityState, EAbilityState InAbilityState)
{
	return InNowAbilityState & static_cast<uint8>(InAbilityState);
}
