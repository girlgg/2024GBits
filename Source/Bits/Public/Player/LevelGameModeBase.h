#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LevelGameModeBase.generated.h"

class UCountdownHUDBase;

UCLASS()
class BITS_API ALevelGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UCountdownHUDBase> CountdownWidgetClass;
	UPROPERTY(Transient, BlueprintReadOnly)
	UCountdownHUDBase* CountdownWidget;
};
