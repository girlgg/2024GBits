#include "Components/Player/InteractionManagerComponent.h"

#include "Common/GameplayFunctinos.h"
#include "Components/Player/InventoryManagerComponent.h"
#include "HUD/InspectHUDBase.h"
#include "Items/InteractiveItemBase.h"
#include "Player/FirstPersonPlayerBase.h"


UInteractionManagerComponent::UInteractionManagerComponent()
{
}

bool UInteractionManagerComponent::CanInteract()
{
	return InteractiveItem != nullptr;
}

void UInteractionManagerComponent::RootInteraction()
{
	if(!CanInteract()) return;
	
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
		InspectUI();
		UGameplayFunctinos::UpdateInputMappingContext(GetWorld(), InspectMapping);
		CurrentInspectState = EInteractionState::Inspecting;
		break;
	case EInteractionMethod::HasItem:
		HasItemInteraction();
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
	if (InteractiveItem)
	{
		InteractiveItem->Outline(false);
		InteractiveItem = nullptr;
	}

	if (AllowInteraction())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
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
				AActor* HitActor = HitResult.GetActor();
				if (HitActor)
				{
					InteractiveItem = Cast<AInteractiveItemBase>(HitActor);
					if (InteractiveItem)
					{
						InteractiveItem->Outline(true);
					}
				}
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InteractCheckTimer,
	                                       this,
	                                       &ThisClass::CheckInteraction, InteractionCheckTime,
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

				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = false;
			}
		}
	}
}

void UInteractionManagerComponent::HasItemInteraction()
{
	if (GetPlayer())
	{
		FString NeedItemName = InteractiveItem->InteractiveData.InteractionMethod.InventoryItemName;
		if (GetPlayer()->InventoryManager->FindItemByName(NeedItemName))
		{
			InteractiveItem->PlayerHasItem();
			if (InteractiveItem->InteractiveData.InteractionMethod.ConsumeItem)
			{
				GetPlayer()->InventoryManager->ReduceItemByNme(NeedItemName);
			}
		}
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
