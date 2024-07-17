// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBaseWidget.generated.h"

class UTileBaseWidget;
class USpatialInventoryComponent;
class UGridPanel;
class UCanvasPanel;
class UWidgetAnimation;

/**
 * The widget to create to display a spactial inventory
 */
UCLASS()
class SPATIALINVENTORY_API UInventoryBaseWidget : public UUserWidget
{
	GENERATED_BODY()
};
