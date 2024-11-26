#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryHUDBase.generated.h"

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

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* InventoryItemSlots;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryItemHUDBase> ItemHUDClass;

private:
	TMap<FString, UInventoryItemHUDBase*> ItemsList;
};
