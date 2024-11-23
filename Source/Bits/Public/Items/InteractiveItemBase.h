#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItemBase.generated.h"

UCLASS()
class BITS_API AInteractiveItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractiveItemBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
