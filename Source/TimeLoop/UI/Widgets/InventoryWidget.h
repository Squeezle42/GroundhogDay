// Copyright (C) 2025 Squeezle Canada
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/TimeLoopBaseWidget.h"
#include "Components/InventoryComponent.h"
#include "InventoryWidget.generated.h"

class UInventoryItemWidget;

/**
 * Inventory widget for the TimeLoop game
 * Displays the player's inventory items
 */
UCLASS()
class TIMELOOP_API UInventoryWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
	// Set the inventory component to display
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent);
	
	// Refresh the inventory display
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void RefreshInventory();
	
	// Called when an item is selected
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OnItemSelected(const FInventoryItem& Item);
	
	// Called when an item is used
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OnItemUsed(const FInventoryItem& Item);
	
	// Called when an item is dropped
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OnItemDropped(const FInventoryItem& Item);
	
protected:
	// The inventory component to display
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	// The container for inventory items
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventoryGrid;
	
	// The selected item info panel
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SelectedItemPanel;
	
	// The selected item name
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectedItemName;
	
	// The selected item description
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectedItemDescription;
	
	// The selected item image
	UPROPERTY(meta = (BindWidget))
	class UImage* SelectedItemImage;
	
	// The use item button
	UPROPERTY(meta = (BindWidget))
	class UButton* UseButton;
	
	// The drop item button
	UPROPERTY(meta = (BindWidget))
	class UButton* DropButton;
	
	// The class to use for inventory item widgets
	UPROPERTY(EditDefaultsOnly, Category = "UI|Inventory")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;
	
	// The currently selected inventory item
	FInventoryItem SelectedItem;
	
	// Called when the Use button is clicked
	UFUNCTION()
	void OnUseButtonClicked();
	
	// Called when the Drop button is clicked
	UFUNCTION()
	void OnDropButtonClicked();
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Called when the inventory changes
	UFUNCTION()
	void OnInventoryChanged();
	
	// Update the selected item panel
	void UpdateSelectedItemPanel();
};
