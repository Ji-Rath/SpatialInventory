// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBaseWidget.h"
#include "SpatialItemData.h"
#include "Components/GridPanel.h"
#include "TileBaseWidget.h"

UItemBaseWidget::UItemBaseWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructGrid(FIntVector2D(3, 3));
}

void UItemBaseWidget::ConstructGrid(FIntVector2D Size)
{
	for (int i = Position.X; i < Size.X+Position.X; i++)
	{
		for (int j = Position.Y; j < Size.Y+Position.Y; j++)
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
