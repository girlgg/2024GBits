#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayFunctinos.generated.h"

class UInputMappingContext;

UCLASS()
class BITS_API UGameplayFunctinos : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void UpdateInputMappingContext(const UObject* WorldContextObject, UInputMappingContext* InInputMapping);
};
