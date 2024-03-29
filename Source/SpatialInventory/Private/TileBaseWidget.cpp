// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBaseWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemBaseWidget.h"
#include "SpatialInventoryComponent.h"
#include "SpatialItemData.h"
#include "InventoryBaseWidget.h"

bool UTileBaseWidget::OnPlaceItem(UDragDropOperation* Operation, FIntVector2D Offset, bool bRotated, int Count)
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

			if (ensure(ItemWidget && OtherInventoryComp && !ItemWidget->ItemData.IsNull() && InventoryComp))
			{
				bSuccess = InventoryComp->AddToSlot(ItemWidget->ItemData.RowHandle, ActualPosition, bRotated, Count);

				// Update widget position value
				ItemWidget->Position = ActualPosition;
				
				// Reconstruct inventory UI
				InventoryWidget->ReconstructItems();
				ItemWidget->InventoryWidget->ReconstructItems();
			}
		}
	}

	return bSuccess;
}
