#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CameraManagerBase.generated.h"

enum class EViewMode : uint8;
class UGeneralHUDBase;

UCLASS()
class BITS_API ACameraManagerBase : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACameraManagerBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void CreateCameraHUD();
	void DestroyCameraHUD();

protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UGeneralHUDBase> GeneralHUDClass{nullptr};
	UPROPERTY(BlueprintReadWrite, Category = UI)
	UGeneralHUDBase* GeneralHUD{nullptr};
};
