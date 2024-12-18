#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/InteractiveItemBase.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryHUDBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BITS_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryManagerComponent();
	void CreateHUD();

	bool AddItemToInventory(const FInteractiveData& InInteractiveData, class AInteractiveItemBase* PendingKill);

	bool FindItemByName(FString& InName);
	void ReduceItemByName(FString& InName);
	FString GetSelectedItemName();

	void Navigate(float Direction);

	void IntoDream();
	void OutDream();

protected:
	virtual void BeginPlay() override;
	void StartGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryHUDBase> InventoryHUDClass;
	UPROPERTY(BlueprintReadOnly)
	UInventoryHUDBase* InventoryHUD{nullptr};

public:
	/* 物体描述 | 个数 */
	UPROPERTY(BlueprintReadOnly)
	TMap<FInteractiveData, int32> InventoryItems;
};
