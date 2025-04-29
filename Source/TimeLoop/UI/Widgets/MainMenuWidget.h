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
#include "MainMenuWidget.generated.h"

/**
 * Main menu widget for the TimeLoop game
 * Handles main menu UI interactions
 */
UCLASS()
class TIMELOOP_API UMainMenuWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
protected:
	// Main menu buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* NewGameButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OptionsButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	
	// Called when the New Game button is clicked
	UFUNCTION()
	void OnNewGameClicked();
	
	// Called when the Continue button is clicked
	UFUNCTION()
	void OnContinueClicked();
	
	// Called when the Options button is clicked
	UFUNCTION()
	void OnOptionsClicked();
	
	// Called when the Quit button is clicked
	UFUNCTION()
	void OnQuitClicked();
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
};
