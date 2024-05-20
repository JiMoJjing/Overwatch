#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IApplyDamageSuccessHandler.generated.h"

UINTERFACE()
class UIApplyDamageSuccessHandler : public UInterface
{
	GENERATED_BODY()
};

class OVERWATCH_API IIApplyDamageSuccessHandler
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void ApplyDamageSuccess(float Damage, bool bIsHeadShot);
};
