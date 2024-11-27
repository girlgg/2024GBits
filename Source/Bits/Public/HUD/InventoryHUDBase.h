﻿#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryHUDBase.generated.h"

class UTextBlock;
struct FInteractiveData;
class UHorizontalBox;
class UInventoryItemHUDBase;

UCLASS()
class BITS_API UInventoryHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// 偷懒，直接写在Tick中，每帧更新物品
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateItem(FString& InItemName, UTexture2D* InItemIcon, int32 InNum);

	void Navigate(float Direction);

	FString GetSelectedItemName();

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* InventoryItemSlots;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* InventoryItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryItemHUDBase> ItemHUDClass;

private:
	TMap<FString, UInventoryItemHUDBase*> ItemsList;
	int32 SelectedItemSlotIndex{0};
	UPROPERTY(Transient)
	UInventoryItemHUDBase* SelectedItemSlot{nullptr};
};
