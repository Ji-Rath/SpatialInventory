// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpatialItemData.h"
#include "InventoryBaseWidget.generated.h"

class USpatialInventoryComponent;
class UGridPanel;
class UCanvasPanel;
class UWidgetAnimation;

/**
 * 
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
	void AddItem(USpatialItemData* ItemData, FIntVector2D Position);

	UFUNCTION(BlueprintCallable)
	void ToggleInventory(bool bOpen, APlayerController* Interactor);

	UFUNCTION(BlueprintCallable)
	void ReconstructItems();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SlotWidget;

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

};
