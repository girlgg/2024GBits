#include "Components/Items/InteractionWidgetComponent.h"

#include "HUD/ItemInteractionHUDBase.h"
#include "Items/InteractiveItemBase.h"


UInteractionWidgetComponent::UInteractionWidgetComponent()
{
}


void UInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = Cast<UItemInteractionHUDBase>(GetUserWidgetObject());
	if (AInteractiveItemBase* Item = Cast<AInteractiveItemBase>(GetOwner()))
	{
		DisplayFromRange = Item->InteractiveData.DisplayPromptFromRange;
		if (CurrentWidget)
		{
			CurrentWidget->UpdateInputInteractionData(Item->InteractiveData);
			CurrentWidget->SetRenderOpacity(0.f);
		}
		SetRelativeLocation(Item->InteractiveData.InteractionPromptOffset);
	}
}