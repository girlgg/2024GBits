#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/InteractiveItemBase.h"
#include "ItemInteractionHUDBase.generated.h"

UCLASS()
class BITS_API UItemInteractionHUDBase : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateInputInteractionData(FInteractiveData& InInteractiveData);
protected:
	FInteractiveData InteractiveData;
};
