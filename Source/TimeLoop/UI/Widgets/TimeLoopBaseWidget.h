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
#include "Blueprint/UserWidget.h"
#include "TimeLoopBaseWidget.generated.h"

/**
 * Base class for all UI widgets in the TimeLoop game
 * Provides common functionality for all widgets
 */
UCLASS()
class TIMELOOP_API UTimeLoopBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTimeLoopBaseWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void InitializeWidget();
	
	// Called when the widget is added to the viewport
	virtual void NativeOnAddedToViewport() override;
	
	// Called when the widget is removed from the viewport
	virtual void NativeOnRemoved() override;
	
	// Play show animation if it exists
	UFUNCTION(BlueprintCallable, Category = "UI|Animation")
	virtual void PlayShowAnimation();
	
	// Play hide animation if it exists
	UFUNCTION(BlueprintCallable, Category = "UI|Animation")
	virtual void PlayHideAnimation();
	
	// Get the player controller owning this widget
	UFUNCTION(BlueprintPure, Category = "UI")
	APlayerController* GetOwningPlayerController() const;
	
protected:
	// Animation played when the widget is shown
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ShowAnimation;
	
	// Animation played when the widget is hidden
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideAnimation;
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Called when the widget is destroyed
	virtual void NativeDestruct() override;
};
