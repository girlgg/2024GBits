#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountdownHUDBase.generated.h"

class UProgressBar;

UCLASS()
class BITS_API UCountdownHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetTime(float InTime);

	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar;

protected:
	UPROPERTY(BlueprintReadOnly)
	float MaxTime{0.f};
	UPROPERTY(BlueprintReadOnly)
	float CurrentTime{0.f};
};
