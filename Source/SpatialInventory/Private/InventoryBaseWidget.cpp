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
	for (int i = 0; i < Size.X; i++)
	{
		for (int j = 0; j < Size.Y; j++)
		{
			UTileBaseWidget* Tile = CreateWidget<UTileBaseWidget>(GetOwningPlayer(), SlotWidget);
			if (Tile)
			{
				Tile->InventoryWidget = this;
				Tile->Position = FIntVector2D(i, j);

				if (GridTiles)
					GridTiles->AddChildToGrid(Tile, j, i);
			}
			
		}
	}
}

void UInventoryBaseWidget::AddItem(USpatialItemData* ItemData, FIntVector2D Position, bool bRotated, int Count)
{
	// Create item widget
	UItemBaseWidget* Item = CreateWidget<UItemBaseWidget>(GetOwningPlayer(), ItemWidget);

	// Set values of new item widget
	Item->Position = Position;
	Item->ItemData = ItemData;
	Item->InventoryWidget = this;
	Item->bItemRotated = bRotated;
	Item->Count = Count;

	// Set proper position of item widget
	UCanvasPanelSlot* CanvasItem = CanvasItems->AddChildToCanvas(Item);
	CanvasItem->SetAutoSize(true);
	CanvasItem->SetPosition(FVector2D(Position.X, Position.Y) * 100);
}

void UInventoryBaseWidget::RefreshInventory(USpatialItemData* ItemData, FIntVector2D Position, bool bRotated, int Count)
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
		if (SlotData.Item != nullptr)
		{
			FIntVector2D Position = InventoryReference->IndexToPos(i);
			AddItem(SlotData.Item, Position, SlotData.bRotated, SlotData.Count);
		}
	}
}

bool UInventoryBaseWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ActorTarget);
	if (Actor)
	{
		InventoryReference = Actor->FindComponentByClass<USpatialInventoryComponent>();
		if (InventoryReference)
		{
			InventoryReference->OnItemAdded.AddDynamic(this, &UInventoryBaseWidget::RefreshInventory);
			InventoryReference->OnToggleInventory.AddDynamic(this, &UInventoryBaseWidget::ToggleInventory);
			ConstructGrid(InventoryReference->InventoryDimensions);
		}
	}

	return bSuccess;
}

UInventoryBaseWidget::UInventoryBaseWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	ConstructGrid(FIntVector2D(3, 3));
}
