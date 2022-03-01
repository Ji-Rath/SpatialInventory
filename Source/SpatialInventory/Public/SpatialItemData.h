// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/ItemData.h"
#include "SpatialItemData.generated.h"

class USpatialItemData;
USTRUCT(BlueprintType)
struct FIntVector2D
{
	GENERATED_USTRUCT_BODY()

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
struct FSlotData
{
	GENERATED_USTRUCT_BODY()

	/** Store a reference of the item data when this is the primary slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpatialItemData* Item = nullptr;

	/** Used when Item is nullptr but the space is still taken */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bTaken = false;

	FSlotData()
	{
		Item = nullptr;

		bTaken = false;
	};

	FSlotData(USpatialItemData* NewItem, bool bIsTaken)
	{
		Item = NewItem;
		bTaken = bIsTaken;
	};
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
	FIntVector2D Size = FIntVector2D();

	/** The value of the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ItemValue = 0.f;

};
