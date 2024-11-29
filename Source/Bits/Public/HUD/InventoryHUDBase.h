#pragma once

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
	void UpdateItem(const FString& InDisplayItemName, const FString& InItemName, UTexture2D* InItemIcon, int32 InNum);

	void Navigate(float Direction);

	UFUNCTION(BlueprintCallable)
	FString GetSelectedItemName();
	UFUNCTION(BlueprintCallable)
	FString GetSelectedItemObjName();
	UFUNCTION(BlueprintCallable)
	FText GetSelectedItemNameText();

	void IntoDream();
	void OutDream();

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* InventoryItemSlots;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryItemHUDBase> ItemHUDClass;

private:
	struct FDisplayHUD
	{
		FString HUDName;
		UInventoryItemHUDBase* HUD;

		bool operator==(const FDisplayHUD& other) const
		{
			return HUD == other.HUD;
		}
	};

	TMap<FString, FDisplayHUD> ItemsList;
	int32 SelectedItemSlotIndex{0};
};
