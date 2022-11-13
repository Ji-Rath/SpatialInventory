// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpatialItemData.h"
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
	
public:
	bool Initialize() override;

	UInventoryBaseWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void ConstructGrid(FIntVector2D Size);

	UFUNCTION(BlueprintCallable)
	void AddItem(const FSlotData& ItemData, const FIntVector2D& Position);

	/** Used to reconstruct inventory when inventory is changed */
	UFUNCTION()
	void RefreshInventory(const FDataTableRowHandle& ItemData, FIntVector2D Position, bool bRotated, int Count);

	UFUNCTION(BlueprintCallable)
	void ToggleInventory(bool bOpen, APlayerController* Interactor);

	UFUNCTION(BlueprintCallable)
	void ReconstructItems();

	UFUNCTION(BlueprintCallable)
	void SetOwner(AActor* NewOwner);

	/**
	 * @brief The widget class that signifies a single grid slot
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SlotWidget;

	/**
	 * @brief The widget class that signifies an item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemWidget;

	UPROPERTY(BlueprintReadWrite)
	USpatialInventoryComponent* InventoryReference = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UGridPanel* GridTiles = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasItems = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnim = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTileBaseWidget*> InventoryTiles;

	/**
	 * @brief The owner to retrieve the spatial inventory component from
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn=true))
	AActor* Owner;
};
