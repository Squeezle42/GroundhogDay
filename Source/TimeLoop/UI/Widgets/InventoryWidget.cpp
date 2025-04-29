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

#include "InventoryWidget.h"
#include "InventoryItemWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Default inventory item widget class
	static ConstructorHelpers::FClassFinder<UInventoryItemWidget> DefaultInventoryItemWidgetClass(TEXT("/Game/UI/Widgets/WBP_InventoryItem"));
	if (DefaultInventoryItemWidgetClass.Succeeded())
	{
		InventoryItemWidgetClass = DefaultInventoryItemWidgetClass.Class;
	}
}

void UInventoryWidget::InitializeWidget()
{
	Super::InitializeWidget();
	
	// Hide the selected item panel initially
	if (SelectedItemPanel)
	{
		SelectedItemPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	
	// Bind button click events
	if (UseButton)
	{
		UseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnUseButtonClicked);
	}
	
	if (DropButton)
	{
		DropButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnDropButtonClicked);
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* InInventoryComponent)
{
	// Unbind from any existing inventory component
	if (InventoryComponent && InventoryComponent->OnInventoryChanged.IsAlreadyBound(this, &UInventoryWidget::OnInventoryChanged))
	{
		InventoryComponent->OnInventoryChanged.RemoveDynamic(this, &UInventoryWidget::OnInventoryChanged);
	}
	
	InventoryComponent = InInventoryComponent;
	
	// Bind to the new inventory component
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::OnInventoryChanged);
	}
	
	// Update the display
	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	if (!InventoryGrid || !InventoryComponent)
	{
		return;
	}
	
	// Clear the inventory grid
	InventoryGrid->ClearChildren();
	
	// Get all items from the inventory component
	TArray<FInventoryItem> InventoryItems = InventoryComponent->GetAllItems();
	
	// Create widgets for each item
	for (const FInventoryItem& Item : InventoryItems)
	{
		if (InventoryItemWidgetClass)
		{
			UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), InventoryItemWidgetClass);
			if (ItemWidget)
			{
				ItemWidget->SetInventoryItem(Item);
				ItemWidget->SetParentInventoryWidget(this);
				InventoryGrid->AddChild(ItemWidget);
			}
		}
	}
}

void UInventoryWidget::OnInventoryChanged()
{
	RefreshInventory();
}

void UInventoryWidget::OnItemSelected(const FInventoryItem& Item)
{
	SelectedItem = Item;
	UpdateSelectedItemPanel();
}

void UInventoryWidget::OnItemUsed(const FInventoryItem& Item)
{
	// This would typically call back to a gameplay system to use the item
	// For now, just log a message
	UE_LOG(LogTemp, Display, TEXT("Item used: %s"), *Item.DisplayName.ToString());
	
	// Refresh the inventory display
	RefreshInventory();
}

void UInventoryWidget::OnItemDropped(const FInventoryItem& Item)
{
	if (InventoryComponent)
	{
		InventoryComponent->RemoveItem(Item.ItemID);
	}
	
	// Hide the selected item panel
	if (SelectedItemPanel)
	{
		SelectedItemPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventoryWidget::OnUseButtonClicked()
{
	OnItemUsed(SelectedItem);
}

void UInventoryWidget::OnDropButtonClicked()
{
	OnItemDropped(SelectedItem);
}

void UInventoryWidget::UpdateSelectedItemPanel()
{
	if (!SelectedItemPanel || SelectedItem.ItemID == NAME_None)
	{
		return;
	}
	
	// Show the selected item panel
	SelectedItemPanel->SetVisibility(ESlateVisibility::Visible);
	
	// Update the item details
	if (SelectedItemName)
	{
		SelectedItemName->SetText(SelectedItem.DisplayName);
	}
	
	if (SelectedItemDescription)
	{
		SelectedItemDescription->SetText(SelectedItem.Description);
	}
	
	if (SelectedItemImage)
	{
		SelectedItemImage->SetBrushFromTexture(SelectedItem.Icon);
	}
}
