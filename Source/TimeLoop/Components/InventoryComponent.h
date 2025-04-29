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
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Forward declarations
class UTexture2D;

/**
 * Inventory item structure
 */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()
	
	// Unique identifier for this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;
	
	// Display name for the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText DisplayName;
	
	// Description of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText Description;
	
	// Icon to display for this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UTexture2D* Icon;
	
	// Whether this item can be stacked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bIsStackable;
	
	// Current stack count for this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 StackCount;
	
	// Maximum stack count for this item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxStackCount;
	
	// Constructor
	FInventoryItem()
		: ItemID(NAME_None)
		, DisplayName(FText::GetEmpty())
		, Description(FText::GetEmpty())
		, Icon(nullptr)
		, bIsStackable(false)
		, StackCount(1)
		, MaxStackCount(1)
	{}
};

/**
 * InventoryComponent - Manages the player's inventory items
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMELOOP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Add an item to the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FInventoryItem& Item);
	
	// Remove an item from the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 Count = 1);
	
	// Check if the inventory has an item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FName ItemID, int32 Count = 1) const;
	
	// Get an item from the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItem GetItem(FName ItemID) const;
	
	// Get all items in the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItem> GetAllItems() const;
	
	// Clear the inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();
	
	// Delegate for inventory changes
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDelegate);
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChangedDelegate OnInventoryChanged;

protected:
	// The items in the inventory
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<FName, FInventoryItem> Items;
	
	// Maximum number of unique items that can be stored
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxItems;
	
	// Find an item by ID
	int32 FindItemIndex(FName ItemID) const;
	
	// Notify that the inventory has changed
	void NotifyInventoryChanged();
};
