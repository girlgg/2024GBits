#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SequenceManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API USequenceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USequenceManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
