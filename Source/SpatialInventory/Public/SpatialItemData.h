// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/ItemData.h"
#include "Inventory/InventoryInfo.h"
#include "SpatialItemData.generated.h"

class UTexture2D;
class USpatialItemData;

// Wrapper for FIntVector2D
USTRUCT(BlueprintType)
struct FIntVector2D
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int X = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Y = 1;

	FIntVector2D()
	{
		X = 0;
		Y = 0;
	};

	FIntVector2D(int XVal, int YVal)
	{
		X = XVal;
		Y = YVal;
	};

	FIntVector2D(int Val)
	{
		X = Val;
		Y = Val;
	};

	FORCEINLINE FIntVector2D operator-(const FIntVector2D& V) const
	{
		return FIntVector2D(X - V.X, Y - V.Y);
	}
	
	FORCEINLINE FIntVector2D operator+(const FIntVector2D& V) const
    	{
    		return FIntVector2D(X + V.X, Y + V.Y);
    	}
	
	FORCEINLINE bool operator==(const FIntVector2D& V) const
	{
		return V.X == X && V.Y == Y;
	}
};

// Information about a specific slot in the grid inventory
USTRUCT(BlueprintType)
struct SPATIALINVENTORY_API FSlotData
{
	GENERATED_BODY()

	// The origin of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector2D Position = FIntVector2D(-1);
};

/**
 * 
 */
UCLASS(BlueprintType)
class SPATIALINVENTORY_API USpatialItemData : public UItemInformation
{
	GENERATED_BODY()

public:
	/** Size that the item takes up in inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntVector2D Dimensions = FIntVector2D();
};
