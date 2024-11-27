#include "HUD/InventoryHUDBase.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Player/InventoryManagerComponent.h"
#include "HUD/InventoryItemHUDBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FirstPersonPlayerBase.h"

void UInventoryHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInventoryHUDBase::UpdateItem(FString& InItemName, UTexture2D* InItemIcon, int32 InNum)
{
	UInventoryItemHUDBase* ItemHUD = nullptr;
	if (InNum <= 0)
	{
		if (ItemsList.Contains(InItemName))
		{
			ItemHUD = *ItemsList.Find(InItemName);
			ItemHUD->RemoveFromParent();
			ItemsList.Remove(InItemName);
		}
		return;
	}

	if (!ItemsList.Contains(InItemName) && IsValid(InventoryItemSlots) && IsValid(ItemHUDClass))
	{
		ItemHUD = CreateWidget<UInventoryItemHUDBase>(GetWorld(), ItemHUDClass);
		if (ItemHUD)
		{
			if (ItemsList.IsEmpty())
			{
				SelectedItemSlot = ItemHUD;
			}
			ItemsList.Add(InItemName, ItemHUD);
			InventoryItemSlots->AddChildToHorizontalBox(ItemHUD);
		}
		ItemHUD->SetPadding(FMargin(2, 0, 2, 0));
	}

	ItemHUD = ItemsList.FindOrAdd(InItemName);

	if (ItemHUD)
	{
		ItemHUD->SetNum(InNum);
		if (InItemIcon)
		{
			ItemHUD->SetIcon(InItemIcon);
		}
	}
}

void UInventoryHUDBase::Navigate(float Direction)
{
	if (Direction > 0)
	{
		++SelectedItemSlotIndex;
	}
	else if (Direction < 0)
	{
		--SelectedItemSlotIndex;
	}
	SelectedItemSlotIndex = FMath::Clamp(SelectedItemSlotIndex, 0, ItemsList.Num() - 1);

	const TArray<UWidget*>& AllItemHUD = InventoryItemSlots->GetAllChildren();
	for (int32 Index = 0; Index < AllItemHUD.Num(); Index++)
	{
		UInventoryItemHUDBase* HUD = Cast<UInventoryItemHUDBase>(AllItemHUD[Index]);
		if (SelectedItemSlotIndex == Index)
		{
			HUD->bIsSelected = true;
			SelectedItemSlot = HUD;
		}
		else
		{
			HUD->bIsSelected = false;
		}
	}
}

FString UInventoryHUDBase::GetSelectedItemName()
{
	if (IsValid(SelectedItemSlot))
	{
		for (auto& Item : ItemsList)
		{
			if (Item.Value == SelectedItemSlot)
			{
				return Item.Key;
			}
		}
	}
	return FString();
}

FText UInventoryHUDBase::GetSelectedItemNameText()
{
	return FText::FromString(GetSelectedItemName());
}
