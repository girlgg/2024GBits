#include "Components/Player/InventoryManagerComponent.h"

#include "Blueprint/UserWidget.h"
#include "HUD/InventoryHUDBase.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
}

bool UInventoryManagerComponent::AddItemToInventory(const FInteractiveData& InInteractiveData,
                                                    AInteractiveItemBase* PendingKill)
{
	auto& Item = InventoryItems.FindOrAdd(InInteractiveData);
	if (Item >= InInteractiveData.InteractionMethod.MaxItemNum)
	{
		// 拾取失败事件
		PendingKill->FailToPickup();
		return false;
	}
	++Item;
	if (IsValid(PendingKill))
	{
		PendingKill->Destroy();
	}
	if (!InInteractiveData.InteractionMethod.bShowInInventory)
	{
		return true;
	}
	if (!IsValid(InventoryHUD))
	{
		CreateHUD();
	}
	if (IsValid(InventoryHUD))
	{
		InventoryHUD->UpdateItem(InInteractiveData.InteractionMethod.PromptText, InInteractiveData.ObjectName,
		                         InInteractiveData.InteractionMethod.InventoryIcon, Item);
	}
	return true;
}

bool UInventoryManagerComponent::FindItemByName(FString& InName)
{
	for (auto& Item : InventoryItems)
	{
		if (Item.Key.ObjectName == InName && Item.Value > 0)
		{
			return true;
		}
	}
	return false;
}

void UInventoryManagerComponent::ReduceItemByName(FString& InName)
{
	for (auto& Item : InventoryItems)
	{
		if (Item.Key.ObjectName == InName)
		{
			--Item.Value;
			if (!Item.Key.InteractionMethod.bShowInInventory)
			{
				return;
			}
			if (!IsValid(InventoryHUD))
			{
				CreateHUD();
			}
			if (IsValid(InventoryHUD))
			{
				InventoryHUD->UpdateItem(Item.Key.InteractionMethod.PromptText,
				                         Item.Key.ObjectName,
				                         Item.Key.InteractionMethod.InventoryIcon, Item.Value);
			}
		}
	}
}

FString UInventoryManagerComponent::GetSelectedItemName()
{
	if (IsValid(InventoryHUD))
	{
		return InventoryHUD->GetSelectedItemObjName();
	}
	return FString();
}

void UInventoryManagerComponent::Navigate(float Direction)
{
	if (IsValid(InventoryHUD))
	{
		InventoryHUD->Navigate(Direction);
	}
}

void UInventoryManagerComponent::IntoDream()
{
	if (InventoryHUD)
	{
		InventoryHUD->IntoDream();
	}
}

void UInventoryManagerComponent::OutDream()
{
	if (InventoryHUD)
	{
		InventoryHUD->OutDream();
	}
}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryManagerComponent::StartGame()
{
	CreateHUD();
}

void UInventoryManagerComponent::CreateHUD()
{
	if (IsValid(InventoryHUDClass))
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			InventoryHUD = CreateWidget<UInventoryHUDBase>(PlayerController, InventoryHUDClass);
			if (InventoryHUD)
			{
				InventoryHUD->AddToViewport();
			}
		}
	}
}
