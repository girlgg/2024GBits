#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UInputMappingContext;

UCLASS()
class BITS_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerControllerBase();
	
	//~ Actor interface
	virtual void BeginPlay() override;
	//~ End of Actor interface
};
