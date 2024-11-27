#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartGameHUDBase.generated.h"

class UTextBlock;

UCLASS()
class BITS_API UStartGameHUDBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateTitleName(FString& InTitle);
	float PlayHideAnim();

protected:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* HideAnim{nullptr};
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelTitle;
};
