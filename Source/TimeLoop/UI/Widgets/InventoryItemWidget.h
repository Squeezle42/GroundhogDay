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
#include "InventoryItemWidget.generated.h"

class UInventoryWidget;

/**
 * Widget for displaying a single inventory item
 */
UCLASS()
class TIMELOOP_API UInventoryItemWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UInventoryItemWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
	// Set the inventory item to display
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void SetInventoryItem(const FInventoryItem& InItem);
	
	// Set the parent inventory widget
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void SetParentInventoryWidget(UInventoryWidget* InParentWidget);
	
	// Get the inventory item
	UFUNCTION(BlueprintPure, Category = "UI|Inventory")
	const FInventoryItem& GetInventoryItem() const { return Item; }
	
protected:
	// The inventory item to display
	UPROPERTY(BlueprintReadOnly, Category = "UI|Inventory")
	FInventoryItem Item;
	
	// The parent inventory widget
	UPROPERTY()
	UInventoryWidget* ParentInventoryWidget;
	
	// The item image
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;
	
	// The item count text (for stacked items)
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountText;
	
	// The button for clicking on the item
	UPROPERTY(meta = (BindWidget))
	class UButton* ItemButton;
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Called when the item button is clicked
	UFUNCTION()
	void OnItemButtonClicked();
};
