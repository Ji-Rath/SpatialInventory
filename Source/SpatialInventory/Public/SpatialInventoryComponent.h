// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpatialItemData.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryInterface.h"
#include "SpatialInventoryComponent.generated.h"

class UGridPanel;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FItemAdded, const FDataTableRowHandle&, Item, FIntVector2D, Position, bool, bRotated, int, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FToggleInventory, bool, bOpen, APlayerController*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotateItem);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPATIALINVENTORY_API USpatialInventoryComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpatialInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector2D InventoryDimensions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlotData> Inventory;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(const FDataTableRowHandle& Item, bool bRotated, int Count = 1);

	UFUNCTION(BlueprintCallable)
	bool AddToSlot(const FDataTableRowHandle& Item, FIntVector2D Position, bool bRotated, int Count = 1);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(const FDataTableRowHandle& Item, FIntVector2D Position, bool bRotated, int Count = 1);

	UFUNCTION(BlueprintCallable)
	bool AddToStack(const FDataTableRowHandle& Item, FIntVector2D Position, int Count);

	UFUNCTION(BlueprintCallable)
	void ClearSlots(TArray<FIntVector2D> Positions);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FIntVector2D> GetSpaceTaken(FIntVector2D Size, FIntVector2D Position, bool bRotated);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasAvailableSpace(FIntVector2D Position, FIntVector2D ItemSize, bool bRotated);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntVector2D IndexToPos(int Index);

	/** Get all items stored in the inventory */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FDataTableRowHandle> GetItems();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int PosToIndex(FIntVector2D Position);

	/** Get the total value of the inventory */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetInventoryValue();

	/** Remove an item from current inventory */
	virtual void RemoveFromInventory_Implementation(const FInventoryContents& Item) override;

	/**
	 * Attempt to add an item to the inventory
	 * @param Item - Item to add
	 * @param Count - Amount of item
	 * @return Whether the item could be added to the inventory
	*/
	virtual bool AddToInventory_Implementation(const FInventoryContents& Item) override;
	
	bool DoesItemExist_Implementation(const FInventoryContents& Item);

	UPROPERTY(BlueprintAssignable)
	FItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FRotateItem OnRotateItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FToggleInventory OnToggleInventory;
};
