#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemHUDBase.generated.h"

class UTextBlock;
class UImage;
class UHorizontalBox;

UCLASS()
class BITS_API UInventoryItemHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNum(int32 InNum);
	void SetIcon(UTexture2D* Icon);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNum;

	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;
};
