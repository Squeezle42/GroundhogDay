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

#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame
	PrimaryComponentTick.bCanEverTick = false;

	// Default inventory size
	MaxItems = 20;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddItem(const FInventoryItem& Item)
{
	if (Item.ItemID == NAME_None)
	{
		return false;
	}
	
	// Check if we already have this item
	if (Items.Contains(Item.ItemID))
	{
		FInventoryItem& ExistingItem = Items[Item.ItemID];
		
		// If stackable, try to add to the stack
		if (ExistingItem.bIsStackable)
		{
			int32 NewStackCount = FMath::Min(ExistingItem.StackCount + Item.StackCount, ExistingItem.MaxStackCount);
			int32 Added = NewStackCount - ExistingItem.StackCount;
			
			ExistingItem.StackCount = NewStackCount;
			
			NotifyInventoryChanged();
			return Added > 0;
		}
		else
		{
			// Not stackable, so we can't add more
			return false;
		}
	}
	else
	{
		// Check if we have space
		if (Items.Num() >= MaxItems)
		{
			return false;
		}
		
		// Add the new item
		Items.Add(Item.ItemID, Item);
		
		NotifyInventoryChanged();
		return true;
	}
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Count)
{
	if (ItemID == NAME_None || Count <= 0 || !Items.Contains(ItemID))
	{
		return false;
	}
	
	FInventoryItem& ExistingItem = Items[ItemID];
	
	if (ExistingItem.bIsStackable)
	{
		// Reduce the stack
		if (ExistingItem.StackCount <= Count)
		{
			// Remove the entire stack
			Items.Remove(ItemID);
		}
		else
		{
			// Reduce the stack count
			ExistingItem.StackCount -= Count;
		}
	}
	else
	{
		// Not stackable, remove the item
		Items.Remove(ItemID);
	}
	
	NotifyInventoryChanged();
	return true;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Count) const
{
	if (ItemID == NAME_None || Count <= 0)
	{
		return false;
	}
	
	if (Items.Contains(ItemID))
	{
		const FInventoryItem& Item = Items[ItemID];
		
		if (Item.bIsStackable)
		{
			return Item.StackCount >= Count;
		}
		else
		{
			return Count == 1;
		}
	}
	
	return false;
}

FInventoryItem UInventoryComponent::GetItem(FName ItemID) const
{
	if (Items.Contains(ItemID))
	{
		return Items[ItemID];
	}
	
	return FInventoryItem();
}

TArray<FInventoryItem> UInventoryComponent::GetAllItems() const
{
	TArray<FInventoryItem> AllItems;
	
	for (const auto& Pair : Items)
	{
		AllItems.Add(Pair.Value);
	}
	
	return AllItems;
}

void UInventoryComponent::ClearInventory()
{
	Items.Empty();
	
	NotifyInventoryChanged();
}

int32 UInventoryComponent::FindItemIndex(FName ItemID) const
{
	int32 Index = 0;
	
	for (const auto& Pair : Items)
	{
		if (Pair.Key == ItemID)
		{
			return Index;
		}
		
		Index++;
	}
	
	return INDEX_NONE;
}

void UInventoryComponent::NotifyInventoryChanged()
{
	OnInventoryChanged.Broadcast();
}
