#include "Items/InteractiveItemBase.h"

#include "Components/SphereComponent.h"
#include "Components/Items/InteractionWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LevelGameModeBase.h"

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

void AInteractiveItemBase::InteractWith()
{
	ExecInteractWith();
	K2_InteractWith();
}

void AInteractiveItemBase::ExecInteractWith()
{
}

void AInteractiveItemBase::BeginPlay()
{
	Super::BeginPlay();

	UICollision->SetSphereRadius(InteractiveData.DisplayPromptFromRange);

	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnIntoDream.AddDynamic(this, &ThisClass::OnIntoDream);
		GM->OnOutDream.AddDynamic(this, &ThisClass::OnOutDream);
	}
}

void AInteractiveItemBase::OnIntoDream()
{
	if (IntoDreamMesh.IsValid())
	{
		Mesh->SetStaticMesh(IntoDreamMesh.LoadSynchronous());
	}
	K2_IntoDream();
}

void AInteractiveItemBase::OnOutDream()
{
	if (OutDreamMesh.IsValid())
	{
		Mesh->SetStaticMesh(OutDreamMesh.LoadSynchronous());
	}
	K2_OutDream();
}

void AInteractiveItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveItemBase::Outline(bool bEnable)
{
	if (bShowOutline == bEnable)
	{
		return;
	}
	bShowOutline = bEnable;
	if (bEnable && bAllowInteract)
	{
		InteractionWidgetComponent->ShowPrompt();
	}
	else
	{
		bShowOutline = false;
		InteractionWidgetComponent->HidePrompt();
	}

	Mesh->SetRenderCustomDepth(bShowOutline);
}

void AInteractiveItemBase::OnPlayerInUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (bAllowInteract)
	{
		InteractionWidgetComponent->ShowPrompt();
	}
}

void AInteractiveItemBase::OnPlayerOutUI(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	InteractionWidgetComponent->HidePrompt();
}
