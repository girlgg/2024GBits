#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayFunctinos.generated.h"

struct FSubtitleSetting;
class UInputMappingContext;

UCLASS()
class BITS_API UGameplayFunctinos : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void UpdateInputMappingContext(const UObject* WorldContextObject, UInputMappingContext* InInputMapping);
	UFUNCTION(BlueprintCallable)
	static void ClearMappingContext(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable)
	static void RestoreMappingContext(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable)
	static void PlaySubtitles(const UObject* WorldContextObject, const TArray<FSubtitleSetting>& Subtitles);
};
