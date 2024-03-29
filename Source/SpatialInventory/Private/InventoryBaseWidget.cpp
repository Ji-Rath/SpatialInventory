// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryBaseWidget.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "SpatialItemData.h"
#include "Kismet/GameplayStatics.h"
#include "SpatialInventoryComponent.h"
#include "ItemBaseWidget.h"
#include "TileBaseWidget.h"

void UInventoryBaseWidget::ConstructGrid(FIntVector2D Size)
{
	InventoryTiles.Empty();
	InventoryTiles.Reserve(Size.X*Size.Y);
	
	for (int i = 0; i < Size.X; i++)
	{
		for (int j = 0; j < Size.Y; j++)
		{
			UTileBaseWidget* Tile = CreateWidget<UTileBaseWidget>(this, SlotWidget);
			if (Tile)
			{
				Tile->InventoryWidget = this;
				Tile->Position = FIntVector2D(i, j);
				InventoryTiles.Add(Tile);

				if (GridTiles)
					GridTiles->AddChildToGrid(Tile, j, i);
			}
			
		}
	}
}

void UInventoryBaseWidget::AddItem(const FSlotData& ItemData, const FIntVector2D& Position)
{
	// Create item widget
	UItemBaseWidget* Item = CreateWidget<UItemBaseWidget>(this, ItemWidget);

	// Set values of new item widget
	Item->Position = Position;
	Item->ItemData = ItemData;
	Item->InventoryWidget = this;

	// Set proper position of item widget
	UCanvasPanelSlot* CanvasItem = CanvasItems->AddChildToCanvas(Item);
	CanvasItem->SetAutoSize(true);
	CanvasItem->SetPosition(FVector2D(Position.X, Position.Y) * 100);
}

void UInventoryBaseWidget::RefreshInventory(const FDataTableRowHandle& ItemData, FIntVector2D Position, bool bRotated, int Count)
{
	ReconstructItems();
}

void UInventoryBaseWidget::ToggleInventory(bool bOpen, APlayerController* Interactor)
{
	if (Interactor)
	{
		SetOwningPlayer(Interactor);
	}

	// Set inventory visibility
	EUMGSequencePlayMode::Type PlayMode = bOpen ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse;
	PlayAnimation(FadeInAnim, 0.0f, 1, PlayMode);

	// Show mouse cursor and ignore look input
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->SetIgnoreLookInput(bOpen);
		bool bInputIgnored = GetOwningPlayer()->IsLookInputIgnored();
		GetOwningPlayer()->bShowMouseCursor = bInputIgnored;
		if (bInputIgnored)
		{
			GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		}
		
	}

	// If the inventory is opened, reconstruct item widgets
	if (bOpen)
	{
		ReconstructItems();
	}
}

void UInventoryBaseWidget::ReconstructItems()
{
	CanvasItems->ClearChildren();
	for(int i = 0; i < InventoryReference->Inventory.Num(); i++)
	{
		FSlotData SlotData = InventoryReference->Inventory[i];
		if (!SlotData.IsNull())
		{
			FIntVector2D Position = InventoryReference->IndexToPos(i);
			AddItem(SlotData, Position);
		}
	}
}

void UInventoryBaseWidget::SetOwner(AActor* NewOwner)
{
	Owner = NewOwner;
	
	if (Owner)
	{
		InventoryReference = Owner->FindComponentByClass<USpatialInventoryComponent>();
		if (InventoryReference)
		{
			InventoryReference->OnItemAdded.AddDynamic(this, &UInventoryBaseWidget::RefreshInventory);
			InventoryReference->OnToggleInventory.AddDynamic(this, &UInventoryBaseWidget::ToggleInventory);
			ConstructGrid(InventoryReference->InventoryDimensions);
		}
	}
}

bool UInventoryBaseWidget::Initialize()
{
	bool bSuccess = Super::Initialize();

	SetOwner(Owner);

	return bSuccess;
}

UInventoryBaseWidget::UInventoryBaseWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructGrid(FIntVector2D(3, 3));
}
