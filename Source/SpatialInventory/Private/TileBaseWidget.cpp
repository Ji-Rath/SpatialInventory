// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBaseWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemBaseWidget.h"
#include "SpatialInventoryComponent.h"
#include "SpatialItemData.h"
#include "InventoryBaseWidget.h"

bool UTileBaseWidget::OnPlaceItem(FGeometry Geometry, FPointerEvent PointerEvent, UDragDropOperation* Operation, FIntVector2D Offset)
{
	bool bSuccess = false;
	FIntVector2D ActualPosition = Position + Offset;

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
				// Ensure the inventory that we are dropping the item to has space
				bool bHasSpace = InventoryComp->HasAvailableSpace(ActualPosition, ItemWidget->ItemData->Size);
				if (bHasSpace)
				{
					InventoryComp->AddToSlot(ItemWidget->ItemData, ActualPosition);

					// Update widget position value
					ItemWidget->Position = ActualPosition;
					
					// Reconstruct inventory UI
					InventoryWidget->ReconstructItems();
					ItemWidget->InventoryWidget->ReconstructItems();

					bSuccess = true;
				}
			}
		}
	}

	return bSuccess;
}
