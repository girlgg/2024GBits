#include "HUD/InventoryItemHUDBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryItemHUDBase::SetNum(int32 InNum)
{
	ItemNum->SetText(FText::AsNumber(InNum));
}

void UInventoryItemHUDBase::SetIcon(UTexture2D* Icon)
{
	ItemImage->SetBrushFromTexture(Icon);
}
