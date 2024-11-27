#include "Components/Player/InteractionManagerComponent.h"

#include "Common/GameplayFunctinos.h"
#include "Components/Player/InventoryManagerComponent.h"
#include "Components/Player/SequenceManagerComponent.h"
#include "HUD/InspectHUDBase.h"
#include "Items/InteractiveItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonPlayerBase.h"
#include "Player/LevelGameModeBase.h"

UInteractionManagerComponent::UInteractionManagerComponent()
{
}

bool UInteractionManagerComponent::CanInteract()
{
	return InteractiveItem != nullptr;
}

void UInteractionManagerComponent::ClickInteraction()
{
	if (InteractiveItem && !InteractiveItem->bIsGazeInteraction)
	{
		RootInteraction();
	}
}

void UInteractionManagerComponent::RootInteraction()
{
	if (!CanInteract()) return;

	FInteractiveData& TargetInteractiveData = InteractiveItem->InteractiveData;

	switch (TargetInteractiveData.InteractionMethod.InteractionMethod)
	{
	case EInteractionMethod::None:
		break;
	case EInteractionMethod::Pickup:
		if (GetPlayer())
		{
			GetPlayer()->InventoryManager->AddItemToInventory(TargetInteractiveData, InteractiveItem);
		}
		break;
	case EInteractionMethod::Inspect:
		switch (TargetInteractiveData.InteractionMethod.InspectMethod)
		{
		case EInspectMethod::None:
			break;
		case EInspectMethod::Readable:
			ReadPaper();
			break;
		case EInspectMethod::Say:
			MakeSay();
			break;
		case EInspectMethod::Max:
			break;
		}
		break;
	case EInteractionMethod::HasItem:
		HasItemInteraction();
		break;
	case EInteractionMethod::IntoDream:
		IntoDream(TargetInteractiveData.InteractionMethod.DreamGetTime);
		InteractiveItem->PlayerHasItem();
		break;
	case EInteractionMethod::OutDream:
		OutDream();
		InteractiveItem->PlayerHasItem();
		break;
	case EInteractionMethod::Door:
		DoorInteraction();
		break;
	case EInteractionMethod::Max:
		break;
	}
}

void UInteractionManagerComponent::CloseInspect()
{
	if (InspectHUD)
	{
		InspectHUD->RemoveFromParent();
	}
	CurrentInspectState = EInteractionState::None;
}

void UInteractionManagerComponent::Navigate(float Direction)
{
	if (IsValid(InspectHUD))
	{
		InspectHUD->NavigateReadablePages(Direction);
	}
}

void UInteractionManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer,
	                                       this,
	                                       &ThisClass::CheckInteraction, InteractionCheckTime,
	                                       false);
}

void UInteractionManagerComponent::CheckInteraction()
{
	LastInteractiveItem = InteractiveItem;

	auto ProcessHitResult = [&](AActor* HitActor)
	{
		if (HitActor)
		{
			InteractiveItem = Cast<AInteractiveItemBase>(HitActor);
			if (InteractiveItem)
			{
				InteractiveItem->Outline(true);
			}
			if (LastInteractiveItem && InteractiveItem != LastInteractiveItem)
			{
				LastInteractiveItem->Outline(false);
			}
		}
	};

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (AllowInteraction() && PlayerController)
	{
		if (bUseMouseLocation)
		{
			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());

			FVector2D MousePosition;
			if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
			{
				if (PlayerController->GetHitResultAtScreenPosition(
					MousePosition, ECC_GameTraceChannel1, Params, HitResult))
				{
					ProcessHitResult(HitResult.GetActor());
				}
			}
		}
		else
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector Start = CameraLocation;
			FVector End = Start + (CameraRotation.Vector() * MaxRayDistance);

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params))
			{
				ProcessHitResult(HitResult.GetActor());
			}
		}
	}

	if (LastInteractiveItem && LastInteractiveItem->bIsGazeInteraction)
	{
		if (LastInteractiveItem == InteractiveItem)
		{
			if (!bIsGazeInteraction)
			{
				CurrentGazeTime += InteractionCheckTime;
				if (CurrentGazeTime > InteractiveItem->GazeDuration)
				{
					CurrentGazeTime = 0.f;
					bIsGazeInteraction = true;
					RootInteraction();
				}
			}
		}
		else
		{
			bIsGazeInteraction = false;
			CurrentGazeTime = 0.f;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer,
	                                       this,
	                                       &ThisClass::CheckInteraction,
	                                       InteractionCheckTime,
	                                       false);
}

bool UInteractionManagerComponent::AllowInteraction()
{
	return bAllowInteraction;
}

void UInteractionManagerComponent::InspectUI()
{
	if (IsValid(InspectHUDClass))
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			InspectHUD = CreateWidget<UInspectHUDBase>(PlayerController, InspectHUDClass);
			if (InspectHUD)
			{
				InspectHUD->AddToViewport();
			}
		}
	}
}

bool UInteractionManagerComponent::HasItemInteraction()
{
	if (GetPlayer())
	{
		FInteractionMethods& InteractionMethod = InteractiveItem->InteractiveData.InteractionMethod;

		FString NeedItemName = InteractiveItem->InteractiveData.InteractionMethod.InventoryItemName;
		if (NeedItemName == TEXT("None") || NeedItemName.IsEmpty())
		{
			InteractiveItem->PlayerHasItem();
		}
		else
		{
			if (InteractionMethod.MustSelectedFirst)
			{
				if (GetPlayer()->InventoryManager->GetSelectedItemName() == NeedItemName)
				{
					InteractiveItem->PlayerHasItem();
					if (InteractiveItem->InteractiveData.InteractionMethod.ConsumeItem)
					{
						GetPlayer()->InventoryManager->ReduceItemByName(NeedItemName);
					}
					return true;
				}
			}
			else
			{
				if (GetPlayer()->InventoryManager->FindItemByName(NeedItemName))
				{
					InteractiveItem->PlayerHasItem();
					if (InteractiveItem->InteractiveData.InteractionMethod.ConsumeItem)
					{
						GetPlayer()->InventoryManager->ReduceItemByName(NeedItemName);
					}
					return true;
				}
			}
		}
	}
	InteractiveItem->K2_NotFound();
	return false;
}

void UInteractionManagerComponent::ReadPaper()
{
	InspectUI();
	if (IsValid(InspectHUD))
	{
		InspectHUD->UpdateReadText(InteractiveItem->InteractiveData.InteractionMethod.ReadableTextPages);
	}
	UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), InspectMapping);
	CurrentInspectState = EInteractionState::Inspecting;
}

void UInteractionManagerComponent::MakeSay()
{
	if (GetPlayer())
	{
		GetPlayer()->SequenceManager->PlaySubtitles(InteractiveItem->InteractiveData.InteractionMethod.SayTextPages);
	}
}

void UInteractionManagerComponent::DoorInteraction()
{
	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		FVector NextPos = GM->GetCurrentRoomPos(InteractiveItem->NextRoomPos);
		GM->ChangePlayerPos(NextPos);
	}
}

void UInteractionManagerComponent::IntoDream(float InDreamTime)
{
	if (GetPlayer())
	{
		GetPlayer()->IntoDream(InDreamTime);
	}
}

void UInteractionManagerComponent::OutDream()
{
	if (GetPlayer())
	{
		GetPlayer()->OutDream();
	}
}

AFirstPersonPlayerBase* UInteractionManagerComponent::GetPlayer()
{
	if (!CurrentPlayer)
	{
		CurrentPlayer = Cast<AFirstPersonPlayerBase>(GetOwner());
	}
	return CurrentPlayer;
}
