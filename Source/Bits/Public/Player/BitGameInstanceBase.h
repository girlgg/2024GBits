#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BitGameInstanceBase.generated.h"


UCLASS()
class BITS_API UBitGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	FString LevelNameToLoad;
};
