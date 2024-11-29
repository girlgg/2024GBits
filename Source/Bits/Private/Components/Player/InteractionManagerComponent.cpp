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
	return InteractiveItem != nullptr && LastInteractiveItem != nullptr &&
		InteractiveItem->bAllowInteract && !bPause;
}

void UInteractionManagerComponent::ClickInteraction()
{
	if (InteractiveItem && !InteractiveItem->bIsGazeInteraction && InteractiveItem->bAllowInteract)
	{
		RootInteraction();
	}
}

void UInteractionManagerComponent::RootInteraction()
{
	if (!CanInteract()) return;

	const FInteractiveData& TargetInteractiveData = InteractiveItem->InteractiveData;

	switch (TargetInteractiveData.InteractionMethod.InteractionMethod)
	{
	case EInteractionMethod::None:
		break;
	case EInteractionMethod::Pickup:
		if (GetPlayer())
		{
			if (GetPlayer()->InventoryManager->AddItemToInventory(TargetInteractiveData, InteractiveItem))
			{
				InteractiveItem->bAllowInteract = false;
			}
		}
		break;
	case EInteractionMethod::Inspect:
		switch (TargetInteractiveData.InteractionMethod.InspectMethod)
		{
		case EInspectMethod::None:
			break;
		case EInspectMethod::Readable:
			ReadPaper();
			HasItemInteraction();
			break;
		case EInspectMethod::Say:
			MakeSay();
			InteractiveItem->bAllowInteract = false;
			HasItemInteraction();
			break;
		case EInspectMethod::Max:
			break;
		}
		break;
	case EInteractionMethod::HasItem:
		HasItemInteraction();
		break;
	case EInteractionMethod::IntoDream:
		if (HasItemInteraction())
		{
			if (GetPlayer())
			{
				GetPlayer()->InventoryManager->AddItemToInventory(TargetInteractiveData, InteractiveItem);
			}
			IntoDream(TargetInteractiveData.InteractionMethod.DreamGetTime);
			InteractiveItem->bAllowInteract = false;
		}
		break;
	case EInteractionMethod::OutDream:
		if (HasItemInteraction())
		{
			if (GetPlayer())
			{
				GetPlayer()->InventoryManager->AddItemToInventory(TargetInteractiveData, InteractiveItem);
			}
			OutDream();
			InteractiveItem->bAllowInteract = false;
		}
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

	if (InteractiveItem && !InteractiveItem->bAllowInteract)
	{
		InteractiveItem->Outline(false);
	}

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
		else
		{
			LastInteractiveItem->Outline(false);
			LastInteractiveItem = nullptr;
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
				else if (LastInteractiveItem)
				{
					LastInteractiveItem->Outline(false);
					LastInteractiveItem = nullptr;
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
			else if (LastInteractiveItem)
			{
				LastInteractiveItem->Outline(false);
				LastInteractiveItem = nullptr;
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
			return true;
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
	if (GetPlayer())
	{
		GetPlayer()->SequenceManager->PlaySubtitles(InteractiveItem->NotFoundPages);
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
	if (GetPlayer() && GetPlayer()->CurrentViewMode == EViewMode::FirstPerson)
	{
		return;
	}
	if (ALevelGameModeBase* GM = Cast<ALevelGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		FVector Dir = (InteractiveItem->GetActorLocation() - GetPlayer()->GetActorLocation()) / 10;
		FVector TarPos = InteractiveItem->GetActorLocation() + Dir;
		const FRoomConfig& Room = GM->GetCurrentRoomPos(TarPos);
		GM->ChangePlayerPos(Room.ThirdPos);
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
