// Fill out your copyright notice in the Description page of Project Settings.


#include "SpatialInventoryComponent.h"
#include "SpatialItemData.h"

// Sets default values for this component's properties
USpatialInventoryComponent::USpatialInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void USpatialInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.AddDefaulted(InventoryDimensions.X * InventoryDimensions.Y);
}


// Called every frame
void USpatialInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USpatialInventoryComponent::TryAddItem(const FDataTableRowHandle& Item, bool bRotated, int Count)
{
	if (ensure(!Item.IsNull()))
	{
		// Try to stack if possible
		int ItemIndex = -1;
		if (Execute_DoesItemExist(this, Item))
		{
			FIntVector2D ItemPosition = IndexToPos(ItemIndex);
			if (AddToSlot(Item, ItemPosition, bRotated, Count))
			{
				OnItemAdded.Broadcast(Item, ItemPosition, bRotated, Count);
				return true;
			}
		}
		
		for (int SlotNum = 0; SlotNum < Inventory.Num(); SlotNum++)
		{
			FIntVector2D Position = IndexToPos(SlotNum);

			// Simply add to inventory
			if (AddToSlot(Item, Position, bRotated, Count))
			{
				OnItemAdded.Broadcast(Item, Position, bRotated, Count);
				return true;
			}
		}
		// Attempt to place item using rotated dimensions
		if (!bRotated)
		{
			return TryAddItem(Item, true, Count);
		}
	}
	return false;
}

bool USpatialInventoryComponent::AddToSlot(const FDataTableRowHandle& Item, FIntVector2D Position, bool bRotated, int Count)
{
	if (AddToStack(Item, Position, Count))
	{
		OnItemAdded.Broadcast(Item, Position, bRotated, Count);
		return true;
	}
	
	bool bCanPlaceItem = HasAvailableSpace(Position, *Item.GetRow<FIntVector2D>("Size"), bRotated);

	if (bCanPlaceItem)
	{
		//Set primary slot properties
		int SlotNum = PosToIndex(Position);
		Inventory[SlotNum].RowHandle = Item;
		Inventory[SlotNum].bRotated = bRotated;
		Inventory[SlotNum].Count = Count;

		//Set child slot properties
		TArray<FIntVector2D> SpaceTaken = GetSpaceTaken(*Item.GetRow<FIntVector2D>("Size"), Position, bRotated);
		for (FIntVector2D SlotPos : SpaceTaken)
		{
			int SlotIndex = PosToIndex(SlotPos);
			Inventory[SlotIndex].ParentIndex = SlotNum;
		}
		
		OnItemAdded.Broadcast(Item, Position, bRotated, Count);
		return true;
	}

	return false;
}

void USpatialInventoryComponent::RemoveItem(const FDataTableRowHandle& Item, FIntVector2D Position, bool bRotated, int Count)
{
	int SlotNum = PosToIndex(Position);
	Inventory[SlotNum].Count -= Count;

	if (Inventory[SlotNum].Count <= 0)
	{
		TArray<FIntVector2D> SpaceTaken = GetSpaceTaken(*Item.GetRow<FIntVector2D>("Size"), Position, bRotated);
		ClearSlots(SpaceTaken);
	}
	
}

bool USpatialInventoryComponent::AddToStack(const FDataTableRowHandle& Item, FIntVector2D Position, int Count)
{
	int Index = PosToIndex(Position);
	if (!Inventory.IsValidIndex(Index)) { return false; }
	
	int ParentIndex = Inventory[Index].ParentIndex;
	
	if (ParentIndex != -1 && Inventory[ParentIndex].RowHandle == Item)
	{
		Inventory[ParentIndex].Count += Count;
		return true;
	}
	return false;
}

void USpatialInventoryComponent::ClearSlots(TArray<FIntVector2D> Positions)
{
	for (FIntVector2D SlotPos : Positions)
	{
		int SlotIndex = PosToIndex(SlotPos);
		Inventory[SlotIndex] = FSlotData();
	}
}

TArray<FIntVector2D> USpatialInventoryComponent::GetSpaceTaken(FIntVector2D Size, FIntVector2D Position, bool bRotated)
{
	TArray<FIntVector2D> FreeSlots;
	int XSize = bRotated ? Size.Y : Size.X;
	int YSize = bRotated ? Size.X : Size.Y;
	
	for (int XPos = Position.X; XPos < XSize + Position.X; XPos++)
	{
		for (int YPos = Position.Y; YPos < YSize + Position.Y; YPos++)
		{
			FreeSlots.Add(FIntVector2D(XPos, YPos));
		}
	}

	return FreeSlots;
}

bool USpatialInventoryComponent::HasAvailableSpace(FIntVector2D Position, FIntVector2D ItemSize, bool bRotated)
{
	TArray<FIntVector2D> FreeSlots;

	int XSize = bRotated ? ItemSize.Y : ItemSize.X;
	int YSize = bRotated ? ItemSize.X : ItemSize.Y;
	
	for (int XPos = Position.X; XPos < XSize+Position.X; XPos++)
	{
		if (XPos >= 0 && XPos < InventoryDimensions.X)
		{
			for (int YPos = Position.Y; YPos < YSize+Position.Y; YPos++)
			{
				//First make sure slot that we are checking is not out of bounds
				if (YPos >= 0 && YPos < InventoryDimensions.Y)
				{
					int IndexToCheck = PosToIndex(FIntVector2D(XPos, YPos));
					FSlotData SlotCheck = Inventory[IndexToCheck];
					if (SlotCheck.IsOccupied())
					{
						return false;
					}
					else
					{
						FreeSlots.Add(FIntVector2D(XPos, YPos));
					}
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

int USpatialInventoryComponent::PosToIndex(FIntVector2D Position)
{
	int ReturnIndex = 0;
	ReturnIndex += Position.X;
	ReturnIndex += Position.Y * InventoryDimensions.X;

	return ReturnIndex;

}

float USpatialInventoryComponent::GetInventoryValue()
{
	float Value = 0.f;
	TArray<FDataTableRowHandle> Items = GetItems();
	for (const FDataTableRowHandle& Item : Items)
	{
		// todo: reimplement this doo doo - throws compile error
		//Value += *(Item.GetRow<float>("ItemValue"));
	}
	return Value;
}

void USpatialInventoryComponent::RemoveFromInventory_Implementation(const FInventoryContents& Item)
{
	IInventoryInterface::RemoveFromInventory_Implementation(Item);
}

bool USpatialInventoryComponent::AddToInventory_Implementation(const FInventoryContents& Item)
{
	return IInventoryInterface::AddToInventory_Implementation(Item);
}

bool USpatialInventoryComponent::DoesItemExist_Implementation(const FInventoryContents& Item)
{
	for(int i=0;i<Inventory.Num();i++)
	{
		FSlotData InventorySlot = Inventory[i];
		if (InventorySlot.RowHandle == Item.RowHandle)
		{
			return true;
		}
	}
	
	return IInventoryInterface::DoesItemExist_Implementation(Item.RowHandle);
}

FIntVector2D USpatialInventoryComponent::IndexToPos(int Index)
{
	FIntVector2D ReturnPosition;

	ReturnPosition.Y = Index / InventoryDimensions.X;
	ReturnPosition.X = Index % InventoryDimensions.X;

	return ReturnPosition;
}

TArray<FDataTableRowHandle> USpatialInventoryComponent::GetItems()
{
	TArray<FDataTableRowHandle> Items;
	for (FSlotData Slot : Inventory)
	{
		if (!Slot.IsNull())
		{
			Items.Add(Slot.RowHandle);
		}
	}
	return Items;
}


