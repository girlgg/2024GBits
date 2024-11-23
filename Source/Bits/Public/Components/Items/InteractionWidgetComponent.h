#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UInteractionWidgetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
