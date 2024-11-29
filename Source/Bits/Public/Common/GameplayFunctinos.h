#pragma once

#include "CoreMinimal.h"
#include "Items/InteractiveItemBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayFunctinos.generated.h"

class AFirstPersonPlayerBase;
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
	UFUNCTION(BlueprintCallable)
	static void AddItemToPlayerInventory(const UObject* WorldContextObject, const FInteractiveData& InInteractiveData);
	UFUNCTION(BlueprintCallable)
	static void MakeGameWin(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable)
	static void SetNextLevelName(const UObject* WorldContextObject, FString LevelName);
	UFUNCTION(BlueprintCallable)
	static void LevelTravel(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable)
	static AFirstPersonPlayerBase* GetPlayer(const UObject* WorldContextObject);
};
