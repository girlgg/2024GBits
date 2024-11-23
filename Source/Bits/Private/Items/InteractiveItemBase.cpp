#include "Items/InteractiveItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/Items/InteractionWidgetComponent.h"

AInteractiveItemBase::AInteractiveItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	InteractionWidgetComponent =
		CreateDefaultSubobject<UInteractionWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(SphereCollision);
}

void AInteractiveItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractiveItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
