#include "Components/Items/InteractionWidgetComponent.h"


UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInteractionWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

