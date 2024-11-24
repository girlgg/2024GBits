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

	UItemInteractionHUDBase* HUD = Cast<UItemInteractionHUDBase>(GetUserWidgetObject());
	AInteractiveItemBase* Item = Cast<AInteractiveItemBase>(GetOwner());

	DisplayFromRange = Item->InteractiveData.DisplayPromptFromRange;
	HUD->UpdateInputInteractionData(Item->InteractiveData);
	SetRelativeLocation(Item->InteractiveData.InteractionPromptOffset);
}


void UInteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

