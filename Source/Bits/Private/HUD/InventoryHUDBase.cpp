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

void UInventoryHUDBase::UpdateItem(const FString& InDisplayItemName, const FString& InItemName, UTexture2D* InItemIcon,
                                   int32 InNum)
{
	if (InNum <= 0)
	{
		if (ItemsList.Contains(InItemName))
		{
			const FDisplayHUD* ItemHUD = ItemsList.Find(InItemName);
			ItemHUD->HUD->RemoveFromParent();
			ItemsList.Remove(InItemName);
		}
		return;
	}

	if (!ItemsList.Contains(InItemName) && IsValid(InventoryItemSlots) && IsValid(ItemHUDClass))
	{
		FDisplayHUD ItemDisplayHUD;
		ItemDisplayHUD.HUD = CreateWidget<UInventoryItemHUDBase>(GetWorld(), ItemHUDClass);
		ItemDisplayHUD.HUDName = InDisplayItemName;
		if (ItemDisplayHUD.HUD)
		{
			ItemsList.Add(InItemName, ItemDisplayHUD);
			InventoryItemSlots->AddChildToHorizontalBox(ItemDisplayHUD.HUD);
		}
		ItemDisplayHUD.HUD->SetPadding(FMargin(2, 0, 2, 0));
	}

	const FDisplayHUD& ItemHUD = ItemsList.FindOrAdd(InItemName);
	ItemHUD.HUD->SetNum(InNum);
	if (InItemIcon)
	{
		ItemHUD.HUD->SetIcon(InItemIcon);
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

	int32 TmpIdx = 0;
	for (auto& Item : ItemsList)
	{
		if (SelectedItemSlotIndex == TmpIdx)
		{
			Item.Value.HUD->bIsSelected = true;
		}
		else
		{
			Item.Value.HUD->bIsSelected = false;
		}
		++TmpIdx;
	}
}

FString UInventoryHUDBase::GetSelectedItemName()
{
	int32 TmpIdx = 0;
	for (auto& Item : ItemsList)
	{
		if (SelectedItemSlotIndex == TmpIdx)
		{
			return Item.Value.HUDName;
		}
		++TmpIdx;
	}
	return FString();
}

FString UInventoryHUDBase::GetSelectedItemObjName()
{
	int32 TmpIdx = 0;
	for (auto& Item : ItemsList)
	{
		if (SelectedItemSlotIndex == TmpIdx)
		{
			return Item.Key;
		}
		++TmpIdx;
	}
	return FString();
}

FText UInventoryHUDBase::GetSelectedItemNameText()
{
	return FText::FromString(GetSelectedItemName());
}

void UInventoryHUDBase::IntoDream()
{
	for (auto& Item : ItemsList)
	{
		if (Item.Value.HUD)
		{
			Item.Value.HUD->IntoDream();
		}
	}
}

void UInventoryHUDBase::OutDream()
{
	for (auto& Item : ItemsList)
	{
		if (Item.Value.HUD)
		{
			Item.Value.HUD->OutDream();
		}
	}
}
