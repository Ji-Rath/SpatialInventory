// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpatialItemData.h"
#include "Inventory/InventoryComponent.h"
#include "SpatialInventoryComponent.generated.h"

class UGridPanel;
class APlayerController;
DECLARE_LOG_CATEGORY_EXTERN(LogSpatialInventory, Log, All);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPATIALINVENTORY_API USpatialInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpatialInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool FindFreeSlot(const FInventoryContents& Item, FIntVector2D& OutPosition) const;

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="IgnoreItems"))
	virtual bool IsSlotTaken(const FIntVector2D& Position, const TArray<FItemHandle>& IgnoreItems) const;

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="IgnoreItems"))
	virtual bool CanPlaceItem(const FIntVector2D& Dimensions, const FIntVector2D& Position, const TArray<FItemHandle>& IgnoreItems) const;
	
	virtual bool CanPlaceItem(const FIntVector2D& Dimensions, const FIntVector2D& Position) const;

	virtual bool CanAddToInventory(const FInventoryContents& Item) const override;

	virtual bool AddToInventory(const FInventoryContents& Item, FItemHandle& OutItemHandle) override;

	virtual FInventoryContents GenerateItem(UItemInformation* ItemInfo, const FInstancedStruct& DynamicData, int Count) const override;

	// Move an item in the inventory. Predicatively moves on the client if possible
	UFUNCTION(BlueprintCallable)
	virtual bool MoveItem(const FItemHandle& Item, const FIntVector2D& NewPosition);

private:
	// Perform move on the server
	UFUNCTION(Server, Reliable)
	virtual void ServerMoveItem(const FItemHandle& Item, const FIntVector2D& NewPosition);

	// Actually perform the move operation
	virtual bool PerformMove(const FItemHandle& Item, const FIntVector2D& NewPosition);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Spatial")
	FIntVector2D InventoryDimensions;
};
