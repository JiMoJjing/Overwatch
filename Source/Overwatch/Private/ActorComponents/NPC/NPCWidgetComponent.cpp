#include "ActorComponents/NPC/NPCWidgetComponent.h"

#include "ActorComponents/Status/HPComponent.h"
#include "Characters/CharacterBase.h"
#include "Characters/NPC/NPCBase.h"
#include "Widgets/NPC/NPCWidget.h"


UNPCWidgetComponent::UNPCWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetWidgetSpace(EWidgetSpace::Screen);
	SetRenderCustomDepth(true);
	SetCustomDepthStencilValue(1);
}

void UNPCWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(UNPCWidget* NPCWidget = Cast<UNPCWidget>(GetUserWidgetObject()))
	{
		if(ANPCBase* Owner = Cast<ANPCBase>(GetOwner()))
		{
			if(UHPComponent* HPComponent = Owner->GetHPComponent())
			{
				HPComponent->OnHPChanged.AddDynamic(NPCWidget, &UNPCWidget::UNPCWidget::OnHPChanged);
				HPComponent->HPChanged();
			}
			if(ACharacterBase* CharacterBase = Cast<ACharacterBase>(NPCWidget->GetOwningPlayerPawn()))
			{
				NPCWidget->SetColorByTeam(Owner->GetTeamID() == CharacterBase->GetTeamID());
				NPCWidget->SetNPCNameTextBlock(Owner->GetNPCName());
			}
		}
	}
}

void UNPCWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

