#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "NPCWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OVERWATCH_API UNPCWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UNPCWidgetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
