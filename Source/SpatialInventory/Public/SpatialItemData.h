// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/ItemData.h"
#include "Inventory/InventoryInfo.h"
#include "SpatialItemData.generated.h"

class UTexture2D;
class USpatialItemData;
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

USTRUCT(BlueprintType)
struct FSlotData : public FInventoryContents
{
	GENERATED_USTRUCT_BODY()

	/** Used to identify the primary slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ParentIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRotated = false;

	FSlotData() {};
	
	FSlotData(const FDataTableRowHandle& Item, int Amount, int Parent)
	{
		RowHandle = Item;
		Count = Amount;
		ParentIndex = Parent;
	}

	bool IsOccupied() const
	{
		return !IsNull() || ParentIndex != -1;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class SPATIALINVENTORY_API USpatialItemData : public UItemData
{
	GENERATED_BODY()

public:

	/** Texture of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	/** Size that the item takes up in inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntVector2D Dimensions = FIntVector2D();

	/** The value of the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ItemValue = 0.f;

};

USTRUCT(BlueprintType)
struct SPATIALINVENTORY_API FSpatialItemInfo : public FItemInfo
{
	GENERATED_BODY();

	/** Texture of item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	/** Size that the item takes up in inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntVector2D Dimensions = FIntVector2D();

	/** The value of the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ItemValue = 0.f;
};
