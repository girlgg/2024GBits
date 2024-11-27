#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InspectHUDBase.generated.h"

class UTextBlock;

UCLASS()
class BITS_API UInspectHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateReadText(const TArray<FString>& InPages);
	void NavigateReadablePages(float Direction);
	void UpdatePageContent(FString InText);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextGuide;

private:
	TArray<FString> Pages;
	int32 CurrentPage{0};
};
