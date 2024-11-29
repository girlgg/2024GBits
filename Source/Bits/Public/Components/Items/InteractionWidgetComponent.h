#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

class UCISInteractionObjectComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInteractionWidgetComponent();

	void ShowPrompt();
	void HidePrompt();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient)
	class UItemInteractionHUDBase* CurrentWidget;

	float DisplayFromRange{0.f};

	int32 CurrentIdx{0};
};
