#include "HUD/InventoryHUDBase.h"

#include "Components/HorizontalBox.h"
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
