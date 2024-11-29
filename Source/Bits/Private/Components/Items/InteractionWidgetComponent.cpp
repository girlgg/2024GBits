#include "Components/Items/InteractionWidgetComponent.h"

#include "HUD/ItemInteractionHUDBase.h"
#include "Items/InteractiveItemBase.h"


UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
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

void UInteractionWidgetComponent::ShowPrompt()
{
	++CurrentIdx;
}

void UInteractionWidgetComponent::HidePrompt()
{
	--CurrentIdx;
	CurrentIdx = FMath::Max(CurrentIdx, 0);
}

void UInteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CurrentWidget)
	{
		return;
	}

	float CurrentOpacity = CurrentWidget->GetRenderOpacity();
	if (CurrentIdx > 0 && CurrentOpacity < .85f)
	{
		CurrentWidget->SetRenderOpacity(FMath::FInterpTo(CurrentOpacity, .85, DeltaTime, 20.f));
	}
	else if (CurrentIdx < 1 && CurrentOpacity > .01f)
	{
		CurrentWidget->SetRenderOpacity(FMath::FInterpTo(CurrentOpacity, 0, DeltaTime, 20.f));
	}
}
