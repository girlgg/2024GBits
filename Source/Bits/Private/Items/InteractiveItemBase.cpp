#include "Items/InteractiveItemBase.h"

AInteractiveItemBase::AInteractiveItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInteractiveItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

