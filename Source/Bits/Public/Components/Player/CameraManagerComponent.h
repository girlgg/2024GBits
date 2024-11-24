#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
