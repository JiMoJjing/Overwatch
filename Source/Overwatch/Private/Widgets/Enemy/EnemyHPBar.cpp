#include "Widgets/Enemy/EnemyHPBar.h"

#include "ActorComponents/Status/HPComponent.h"
#include "Characters/CharacterBase.h"
#include "Components/ProgressBar.h"

#include "Utilities.h"

void UEnemyHPBar::NativeConstruct()
{
	Super::NativeConstruct();

}

void UEnemyHPBar::OnHPChanged(float InCurrentHP, float InMaxHP)
{
	float Percentage = InCurrentHP / InMaxHP;
	HPBar->SetPercent(Percentage);
}
