// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBaseWidget.h"
#include "SpatialItemData.h"
#include "Components/GridPanel.h"
#include "TileBaseWidget.h"

void UItemBaseWidget::ConstructGrid(FIntVector2D Size, bool bRotated)
{
	GridTiles->ClearChildren();
	int XSize = bRotated ? Size.Y : Size.X;
	int YSize = bRotated ? Size.X : Size.Y;
	
	for (int i = Position.X; i < XSize+Position.X; i++)
	{
		for (int j = Position.Y; j < YSize+Position.Y; j++)
		{
			auto* Tile = CreateWidget<UTileBaseWidget>(GetOwningPlayer(), SlotWidget);
			if (Tile)
			{
				Tile->InventoryWidget = InventoryWidget;
				Tile->Position = FIntVector2D(i, j);

				if (GridTiles)
					GridTiles->AddChildToGrid(Tile, j, i);
			}
		}
	}
}
