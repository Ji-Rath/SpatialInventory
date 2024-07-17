// Fill out your copyright notice in the Description page of Project Settings.


#include "SpatialInventoryComponent.h"
#include "SpatialItemData.h"

DEFINE_LOG_CATEGORY(LogSpatialInventory)

// Sets default values for this component's properties
USpatialInventoryComponent::USpatialInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InventoryDimensions = FIntVector2D(10, 10);
}


// Called when the game starts
void USpatialInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USpatialInventoryComponent::FindFreeSlot(const FInventoryContents& Item, FIntVector2D& OutPosition) const
{
	for(int y=0;y<InventoryDimensions.Y;y++)
	{
		for(int x=0;x<InventoryDimensions.X;x++)
		{
			auto SpatialData = Item.GetItemInformation<USpatialItemData>();
			if (CanPlaceItem(SpatialData->Dimensions, FIntVector2D(x, y)))
			{
				OutPosition = FIntVector2D(x, y);
				return true;
			}
		}
	}
	return false;
}

bool USpatialInventoryComponent::IsSlotTaken(const FIntVector2D& Position) const
{
	for (const auto& InventoryContents : Inventory)
	{
		auto StaticData = Cast<USpatialItemData>(InventoryContents.ItemInformation);
		if (!StaticData) { return false; }
		
		const auto& SlotData = InventoryContents.DynamicData.Get<FSlotData>();
		auto ItemPosition = SlotData.Position;
		auto Dimensions = StaticData->Dimensions;

		// If position is not valid (item has not been placed in inventory), skip it
		if (ItemPosition.X < 0 || ItemPosition.Y < 0) { continue; }

		bool bWithinXBounds = Position.X >= ItemPosition.X && Position.X < (Dimensions.X+ItemPosition.X);
		bool bWithinYBounds = Position.Y >= ItemPosition.Y && Position.Y < (Dimensions.Y+ItemPosition.Y);
		
		if (bWithinXBounds && bWithinYBounds)
		{
			return true;
		}
	}
	
	return false;
}

bool USpatialInventoryComponent::CanPlaceItem(const FIntVector2D& Dimensions, const FIntVector2D& Position) const
{
	for(int x=0;x<Dimensions.X;x++)
	{
		for(int y=0;y<Dimensions.Y;y++)
		{
			bool bOutOfBounds = Position.X + x >= InventoryDimensions.X || Position.Y + y >= InventoryDimensions.Y;
			if (bOutOfBounds || IsSlotTaken(Position + FIntVector2D(x, y)))
			{
				return false;
			}
		}
	}
	
	return true;
}

bool USpatialInventoryComponent::CanAddToInventory(const FInventoryContents& Item) const
{
	FIntVector2D Position;
	return FindFreeSlot(Item, Position);
}

bool USpatialInventoryComponent::AddToInventory(const FInventoryContents& Item, FItemHandle& OutItemHandle)
{
	if (Super::AddToInventory(Item, OutItemHandle))
	{
		if (auto ItemData = Inventory.FindByKey(OutItemHandle))
		{
			FIntVector2D Position;
			FindFreeSlot(Item, Position);
			ItemData->DynamicData.GetMutable<FSlotData>().Position = Position;
			UE_LOG(LogSpatialInventory, Log, TEXT("Added item %s to inventory at (%d, %d)"), *Item.ItemInformation->DisplayName.ToString(), Position.X, Position.Y);
			return true;
		}
	}
	return false;
}

FInventoryContents USpatialInventoryComponent::GenerateItem(UItemInformation* ItemInfo,
	const FInstancedStruct& DynamicData, int Count) const
{
	check(ItemInfo->IsA<USpatialItemData>())
	
	FInstancedStruct NewDynamicData = DynamicData;
	if (!DynamicData.IsValid())
	{
		NewDynamicData = FInstancedStruct(FSlotData::StaticStruct());
	}
	
	return Super::GenerateItem(ItemInfo, NewDynamicData, Count);
}
