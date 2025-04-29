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

#include "InventoryItemWidget.h"
#include "InventoryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UInventoryItemWidget::InitializeWidget()
{
	Super::InitializeWidget();
}

void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Bind button click event
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UInventoryItemWidget::OnItemButtonClicked);
	}
}

void UInventoryItemWidget::SetInventoryItem(const FInventoryItem& InItem)
{
	Item = InItem;
	
	// Update the UI
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(Item.Icon);
	}
	
	if (CountText)
	{
		if (Item.bIsStackable && Item.StackCount > 1)
		{
			CountText->SetText(FText::AsNumber(Item.StackCount));
			CountText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CountText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInventoryItemWidget::SetParentInventoryWidget(UInventoryWidget* InParentWidget)
{
	ParentInventoryWidget = InParentWidget;
}

void UInventoryItemWidget::OnItemButtonClicked()
{
	if (ParentInventoryWidget)
	{
		ParentInventoryWidget->OnItemSelected(Item);
	}
}
