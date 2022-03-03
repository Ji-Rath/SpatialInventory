// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpatialItemData.h"
#include "TileBaseWidget.generated.h"

class UInventoryBaseWidget;

/**
 * 
 */
UCLASS()
class SPATIALINVENTORY_API UTileBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	bool OnPlaceItem(UDragDropOperation* Operation, FIntVector2D Offset, bool bRotated);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryBaseWidget* InventoryWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FIntVector2D Position;
};
