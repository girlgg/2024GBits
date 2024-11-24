#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneralHUDBase.generated.h"

UCLASS()
class BITS_API UGeneralHUDBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FPSText;

	UPROPERTY(meta = (BindWidget))
	class UImage* CursorImageA;
	UPROPERTY(meta = (BindWidget))
	class UImage* CursorImageB;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CanInteractCursorAnim{nullptr};
private:
	bool bLastCanInteract{false};
};
