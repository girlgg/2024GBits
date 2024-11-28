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

	void IntoDream();
	void OutDream();

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected{false};

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNum;

	UPROPERTY(meta=(BindWidget))
	UImage* ItemImage;

	/* 现实背景 */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BackgroundT;

	/* 梦境背景 */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* BackgroundD;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* BackgroundImage;
};
