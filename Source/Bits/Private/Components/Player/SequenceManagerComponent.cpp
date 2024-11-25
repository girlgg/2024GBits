#include "Components/Player/SequenceManagerComponent.h"


USequenceManagerComponent::USequenceManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USequenceManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USequenceManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

