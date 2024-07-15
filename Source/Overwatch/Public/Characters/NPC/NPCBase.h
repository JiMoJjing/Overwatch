#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "NPCBase.generated.h"

class UNPCWidgetComponent;
class ADeflectCollider;

UCLASS()
class OVERWATCH_API ANPCBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	ANPCBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE FText GetNPCName() const { return NPCName; }

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void CharacterRevive() override;
	virtual void CharacterDeath() override;
	virtual void NotifyCharacterDeath(AController* EventInstigator, AActor* DamageCauser, bool bIsHeadShot) override;
	
	void SetNPCWidgetVisibility(ESlateVisibility InVisibility);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "NPCBase", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNPCWidgetComponent> NPCWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "NPCBase", meta = (AllowPrivateAccess = "true"))
	FText NPCName;
};
