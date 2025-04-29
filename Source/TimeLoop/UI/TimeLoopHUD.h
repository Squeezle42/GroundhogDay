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
#include "GameFramework/HUD.h"
#include "TimeLoopHUD.generated.h"

class UTimeLoopBaseWidget;
class UGameHUDWidget;
class UMainMenuWidget;
class UInventoryWidget;
class UDialogueWidget;
class UEndGameCreditsWidget;
class UGameStartWidget;

/**
 * TimeLoopHUD - Main HUD class for the TimeLoop game
 * Manages various UI widgets and their visibility
 */
UCLASS()
class TIMELOOP_API ATimeLoopHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Sets default values
	ATimeLoopHUD();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void DrawHUD() override;

	// Shows or hides the main menu
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleMainMenu();
	
	// Shows or hides the inventory
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleInventory();
		// Shows or hides the dialogue UI
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowDialogueUI(bool bShow);
		// Shows the end game credits UI
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowEndGameCredits();
	
	// Shows the game start sequence
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameStartSequence();
	
	// Gets the main game HUD widget
	UFUNCTION(BlueprintPure, Category = "UI")
	UGameHUDWidget* GetGameHUD() const { return GameHUDWidget; }
	
	// Gets the main menu widget
	UFUNCTION(BlueprintPure, Category = "UI")
	UMainMenuWidget* GetMainMenu() const { return MainMenuWidget; }
	
	// Gets the inventory widget
	UFUNCTION(BlueprintPure, Category = "UI")
	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	
	// Gets the dialogue widget
	UFUNCTION(BlueprintPure, Category = "UI")
	UDialogueWidget* GetDialogueWidget() const { return DialogueWidget; }
		// Gets the end game credits widget
	UFUNCTION(BlueprintPure, Category = "UI")
	UEndGameCreditsWidget* GetEndGameCreditsWidget() const { return EndGameCreditsWidget; }
	
	// Gets the game start widget
	UFUNCTION(BlueprintPure, Category = "UI")
	UGameStartWidget* GetGameStartWidget() const { return GameStartWidget; }

protected:
	// The class to use for the game HUD widget
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UGameHUDWidget> GameHUDWidgetClass;
	
	// The class to use for the main menu widget
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
	
	// The class to use for the inventory widget
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
		// The class to use for the dialogue widget
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	// The class to use for the end game credits widget
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<UEndGameCreditsWidget> EndGameCreditsWidgetClass;

private:
	// The active game HUD widget instance
	UPROPERTY()
	UGameHUDWidget* GameHUDWidget;
	
	// The active main menu widget instance
	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;
	
	// The active inventory widget instance
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	
	// The active dialogue widget instance
	UPROPERTY()
	UDialogueWidget* DialogueWidget;
	
	// The end game credits widget instance
	UPROPERTY()
	UEndGameCreditsWidget* EndGameCreditsWidget;
	
	// Initialize a widget of the specified class and add it to the viewport
	template<typename T>
	T* CreateWidget(TSubclassOf<T> WidgetClass, bool bAddToViewport = true);
};
