#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogResponse.generated.h"

UCLASS()
class BITS_API UDialogResponse : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateWidgetReplies(FString& InA, FString& InB);
protected:
	UPROPERTY(BlueprintReadOnly)
	FString ReplyA;
	UPROPERTY(BlueprintReadOnly)
	FString ReplyB;
};
