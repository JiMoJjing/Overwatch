#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IKillEventHandler.generated.h"

UINTERFACE()
class UIKillEventHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OVERWATCH_API IIKillEventHandler
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void KillEvent();
};
