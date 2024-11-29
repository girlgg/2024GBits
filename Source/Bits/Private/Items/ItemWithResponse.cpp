#include "Items/ItemWithResponse.h"

#include "Common/GameplayFunctinos.h"
#include "Components/Items/DialogComponent.h"
#include "Components/Player/InteractionManagerComponent.h"
#include "Player/FirstPersonPlayerBase.h"


AItemWithResponse::AItemWithResponse()
{
	PrimaryActorTick.bCanEverTick = true;

	DialogComponent = CreateDefaultSubobject<UDialogComponent>(TEXT("DialogComponent"));
}

void AItemWithResponse::ExecInteractWith()
{
	Super::ExecInteractWith();
	if (DialogComponent)
	{
		DialogComponent->InitializeConversation(this, DialogSets);
	}
}

void AItemWithResponse::InputReply(int32 OptionAction)
{
	if (DialogComponent)
	{
		DialogComponent->InputReply(OptionAction);
	}
}

void AItemWithResponse::InputEndRelies()
{
	if (DialogComponent)
	{
		DialogComponent->StopConversation();
	}
	UGameplayFunctinos::GetPlayer(GetWorld())->InteractionManager->ResponseItem = nullptr;
}

void AItemWithResponse::BeginPlay()
{
	Super::BeginPlay();
}

void AItemWithResponse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
