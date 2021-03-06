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

bool USpatialInventoryComponent::TryAddItem(USpatialItemData* Item, bool bRotated)
{
	if (ensure(Item))
	{
		for (int SlotNum = 0; SlotNum < Inventory.Num(); SlotNum++)
		{
			FSlotData Slot = Inventory[SlotNum];
			FIntVector2D Position = IndexToPos(SlotNum);
			bool bCanPlaceItem = HasAvailableSpace(Position, Item->Size, bRotated);

			if (bCanPlaceItem)
			{
				Inventory[SlotNum].Item = Item;
				Inventory[SlotNum].bRotated = bRotated;
				TArray<FIntVector2D> SpaceTaken = GetSpaceTaken(Item->Size, Position, bRotated);
				SetOccupied(true, SpaceTaken);
				
				OnItemAdded.Broadcast(Item, Position, bRotated);
				return true;
			}
		}
		// Attempt to place item using rotated dimensions
		if (!bRotated)
		{
			return TryAddItem(Item, true);
		}
	}
	return false;
}

bool USpatialInventoryComponent::AddToSlot(USpatialItemData* Item, FIntVector2D Position, bool bRotated)
{
	bool bCanPlaceItem = HasAvailableSpace(Position, Item->Size, bRotated);

	if (bCanPlaceItem)
	{
		int SlotNum = PosToIndex(Position);
		Inventory[SlotNum].Item = Item;
		Inventory[SlotNum].bRotated = bRotated;
		
		TArray<FIntVector2D> SpaceTaken = GetSpaceTaken(Item->Size, Position, bRotated);
		SetOccupied(true, SpaceTaken);
		
		OnItemAdded.Broadcast(Item, Position, bRotated);
		return true;
	}

	return false;
}

void USpatialInventoryComponent::RemoveItem(USpatialItemData* Item, FIntVector2D Position, bool bRotated)
{
	int SlotNum = PosToIndex(Position);
	Inventory[SlotNum].Item = nullptr;

	TArray<FIntVector2D> SpaceTaken = GetSpaceTaken(Item->Size, Position, bRotated);
	SetOccupied(false, SpaceTaken);
}

void USpatialInventoryComponent::SetOccupied(bool bOccupied, TArray<FIntVector2D> Positions)
{
	for (FIntVector2D SlotPos : Positions)
	{
		int SlotIndex = PosToIndex(SlotPos);
		Inventory[SlotIndex].bTaken = bOccupied;
		if (!bOccupied)
		{
			Inventory[SlotIndex].Item = nullptr;
		}
			
		//UE_LOG(LogTemp, Log, TEXT("SLOT (%d, %d) HAS NOW BEEN TAKEN!"), SlotPos.X, SlotPos.Y);
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
					if (SlotCheck.bTaken)
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
	TArray<USpatialItemData*> Items = GetItems();
	for (USpatialItemData* Item : Items)
	{
		Value += Item->ItemValue;
	}
	return Value;
}

void USpatialInventoryComponent::RemoveFromInventory_Implementation(const UItemData* Item, const int Count)
{
	IInventoryInterface::RemoveFromInventory_Implementation(Item, Count);
}

bool USpatialInventoryComponent::AddToInventory_Implementation(UItemData* Item, const int Count)
{
	return IInventoryInterface::AddToInventory_Implementation(Item, Count);
}

bool USpatialInventoryComponent::DoesItemExist_Implementation(UItemData* Item)
{
	return IInventoryInterface::DoesItemExist_Implementation(Item);
}

FIntVector2D USpatialInventoryComponent::IndexToPos(int Index)
{
	FIntVector2D ReturnPosition;

	ReturnPosition.Y = Index / InventoryDimensions.X;
	ReturnPosition.X = Index % InventoryDimensions.X;

	return ReturnPosition;
}

TArray<USpatialItemData*> USpatialInventoryComponent::GetItems()
{
	TArray<USpatialItemData*> Items;
	for (FSlotData Slot : Inventory)
	{
		if (Slot.Item != nullptr)
		{
			Items.Add(Slot.Item);
		}
	}
	return Items;
}


