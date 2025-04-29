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
#include "GameHUDWidget.generated.h"

/**
 * Game HUD widget for the TimeLoop game
 * Displays in-game information like time, objectives, and interaction prompts
 */
UCLASS()
class TIMELOOP_API UGameHUDWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UGameHUDWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
	// Update the current time display
	UFUNCTION(BlueprintCallable, Category = "UI|Time")
	void UpdateTimeDisplay(int32 Day, int32 Hour, int32 Minute);
	
	// Show an interaction prompt
	UFUNCTION(BlueprintCallable, Category = "UI|Interaction")
	void ShowInteractionPrompt(const FText& PromptText, const FText& ActionText);
	
	// Hide the interaction prompt
	UFUNCTION(BlueprintCallable, Category = "UI|Interaction")
	void HideInteractionPrompt();
	
	// Show a notification message
	UFUNCTION(BlueprintCallable, Category = "UI|Notification")
	void ShowNotification(const FText& NotificationText, float Duration = 3.0f);
	
	// Update the current objective text
	UFUNCTION(BlueprintCallable, Category = "UI|Objectives")
	void UpdateObjective(const FText& ObjectiveText);

protected:
	// Time display widget
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;
	
	// Day counter widget
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DayText;
	
	// Interaction prompt container
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* InteractionPromptContainer;
	
	// Interaction prompt text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractionPromptText;
	
	// Interaction action text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractionActionText;
	
	// Notification text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotificationText;
	
	// Current objective text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentObjectiveText;
	
	// Timer handle for hiding notifications
	FTimerHandle NotificationTimerHandle;
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Hide the notification text after a delay
	UFUNCTION()
	void HideNotification();
};
