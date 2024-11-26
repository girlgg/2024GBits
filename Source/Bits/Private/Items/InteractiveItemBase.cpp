#include "Items/InteractiveItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/Items/InteractionWidgetComponent.h"
#include "HUD/ItemInteractionHUDBase.h"

AInteractiveItemBase::AInteractiveItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	UICollision = CreateDefaultSubobject<USphereComponent>(TEXT("UICollision"));
	UICollision->SetupAttachment(SphereCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	InteractionWidgetComponent =
		CreateDefaultSubobject<UInteractionWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(SphereCollision);

	SphereCollision->SetSphereRadius(CustomCollisionRadius);
	if (IsValid(InteractiveData.ObjectMesh))
	{
		Mesh->SetStaticMesh(InteractiveData.ObjectMesh);
	}

	UICollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPlayerInUI);
	UICollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnPlayerOutUI);
}

void AInteractiveItemBase::BeginPlay()
{
	Super::BeginPlay();

	UICollision->SetSphereRadius(InteractiveData.DisplayPromptFromRange);
}

void AInteractiveItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveItemBase::Outline(bool bEnable)
{
	Mesh->SetRenderCustomDepth(bEnable);
}

void AInteractiveItemBase::OnPlayerInUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	InteractionWidgetComponent->CurrentWidget->SetRenderOpacity(.85f);
}

void AInteractiveItemBase::OnPlayerOutUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	InteractionWidgetComponent->CurrentWidget->SetRenderOpacity(.0f);
}
