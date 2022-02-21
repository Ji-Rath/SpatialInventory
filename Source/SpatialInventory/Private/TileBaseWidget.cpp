// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBaseWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemBaseWidget.h"
#include "SpatialInventoryComponent.h"
#include "SpatialItemData.h"
#include "InventoryBaseWidget.h"

bool UTileBaseWidget::OnPlaceItem(FGeometry Geometry, FPointerEvent PointerEvent, UDragDropOperation* Operation)
{
	bool bSuccess = false;

	if (ensure(Operation && IsValid(Operation->Payload) && InventoryWidget))
	{
		// Retrieve payload from drag drop operation
		UItemBaseWidget* ItemWidget = Cast<UItemBaseWidget>(Operation->Payload);

		if (ensure(ItemWidget && ItemWidget->InventoryWidget))
		{
			// Get inventory component references
			USpatialInventoryComponent* OtherInventoryComp = ItemWidget->InventoryWidget->InventoryReference;
			USpatialInventoryComponent* InventoryComp = InventoryWidget->InventoryReference;

			if (ensure(ItemWidget && OtherInventoryComp && ItemWidget->ItemData && InventoryComp))
			{
				// Clear space from old inventory
				TArray<FIntVector2D> SpaceTaken = OtherInventoryComp->GetSpaceTaken(ItemWidget->ItemData->Size, ItemWidget->Position);
				OtherInventoryComp->SetOccupied(false, SpaceTaken);

				// Ensure the inventory that we are dropping the item to has space
				bool bHasSpace = InventoryComp->HasAvailableSpace(Position, ItemWidget->ItemData->Size);
				if (bHasSpace)
				{
					// Occupy new space
					SpaceTaken = InventoryComp->GetSpaceTaken(ItemWidget->ItemData->Size, Position);
					InventoryComp->SetOccupied(true, SpaceTaken);

					// Update Inventory array
					int InvIndex = InventoryComp->PosToIndex(Position);
					InventoryComp->Inventory[InvIndex] = FSlotData(ItemWidget->ItemData, true);

					// Update widget position value
					ItemWidget->Position = Position;

					// Reconstruct inventory UI
					InventoryWidget->ReconstructItems();
					ItemWidget->InventoryWidget->ReconstructItems();

					bSuccess = true;
				}
				else
				{
					OtherInventoryComp->SetOccupied(true, SpaceTaken);
				}
			}
		}
	}

	return bSuccess;
}
