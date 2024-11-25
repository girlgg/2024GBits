#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SequenceManagerComponent.generated.h"

struct FSubtitleSetting;
class USequenceHUDBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API USequenceManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USequenceManagerComponent();

	UFUNCTION(BlueprintCallable)
	void PlaySubtitles(const TArray<FSubtitleSetting>& Subtitles);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USequenceHUDBase> SequenceHUDClass;
	UPROPERTY(Transient)
	USequenceHUDBase* SequenceHUD;
};
