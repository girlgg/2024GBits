#include "Components/Player/InventoryManagerComponent.h"

#include "Blueprint/UserWidget.h"
#include "HUD/InventoryHUDBase.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
}

void UInventoryManagerComponent::AddItemToInventory(FInteractiveData& InInteractiveData,
                                                    AInteractiveItemBase* PendingKill)
{
	auto& Item = InventoryItems.FindOrAdd(InInteractiveData);
	if (Item >= InInteractiveData.InteractionMethod.MaxItemNum)
	{
		// TODO：触发拾取失败事件
		return;
	}
	++Item;
	if (IsValid(PendingKill))
	{
		PendingKill->Destroy();
	}
	if (!InInteractiveData.InteractionMethod.bShowInInventory)
	{
		return;
	}
	if (!IsValid(InventoryHUD))
	{
		CreateHUD();
	}
	if (IsValid(InventoryHUD))
	{
		InventoryHUD->UpdateItem(InInteractiveData.ObjectName, InInteractiveData.InteractionMethod.InventoryIcon, Item);
	}
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
				InventoryHUD->UpdateItem(Item.Key.ObjectName, Item.Key.InteractionMethod.InventoryIcon, Item.Value);
			}
		}
	}
}

FString UInventoryManagerComponent::GetSelectedItemName()
{
	if (IsValid(InventoryHUD))
	{
		return InventoryHUD->GetSelectedItemName();
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
