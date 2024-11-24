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

	void AddItemToInventory(FInteractiveData& InInteractiveData, class AInteractiveItemBase* PendingKill);

	bool FindItemByName(FString& InName);
	void ReduceItemByNme(FString& InName);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryHUDBase> InventoryHUDClass{nullptr};
	UPROPERTY(BlueprintReadOnly)
	UInventoryHUDBase* InventoryHUD{nullptr};

	/* 物体描述 | 个数 */
	UPROPERTY(BlueprintReadOnly)
	TMap<FInteractiveData, int32> InventoryItems;
};
