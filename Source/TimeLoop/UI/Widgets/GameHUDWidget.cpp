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

#include "GameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

UGameHUDWidget::UGameHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameHUDWidget::InitializeWidget()
{
	Super::InitializeWidget();
	
	// Initialize with default values
	UpdateTimeDisplay(1, 8, 0); // Start at day 1, 8:00 AM
	HideInteractionPrompt();
	
	// Hide notification text initially
	if (NotificationText)
	{
		NotificationText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	// Set default objective
	UpdateObjective(FText::FromString("Find your way around town"));
}

void UGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGameHUDWidget::UpdateTimeDisplay(int32 Day, int32 Hour, int32 Minute)
{
	if (TimeText)
	{
		FString AMPM = Hour < 12 ? TEXT("AM") : TEXT("PM");
		int32 Hour12 = Hour % 12;
		if (Hour12 == 0) Hour12 = 12; // Convert 0 to 12 for 12-hour format
		
		FString TimeString = FString::Printf(TEXT("%02d:%02d %s"), Hour12, Minute, *AMPM);
		TimeText->SetText(FText::FromString(TimeString));
	}
	
	if (DayText)
	{
		FString DayString = FString::Printf(TEXT("Day %d"), Day);
		DayText->SetText(FText::FromString(DayString));
	}
}

void UGameHUDWidget::ShowInteractionPrompt(const FText& PromptText, const FText& ActionText)
{
	if (InteractionPromptContainer)
	{
		InteractionPromptContainer->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (InteractionPromptText)
	{
		InteractionPromptText->SetText(PromptText);
	}
	
	if (InteractionActionText)
	{
		InteractionActionText->SetText(ActionText);
	}
}

void UGameHUDWidget::HideInteractionPrompt()
{
	if (InteractionPromptContainer)
	{
		InteractionPromptContainer->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameHUDWidget::ShowNotification(const FText& NotificationText, float Duration)
{
	if (this->NotificationText)
	{
		this->NotificationText->SetText(NotificationText);
		this->NotificationText->SetVisibility(ESlateVisibility::Visible);
		
		// Clear any existing timer
		if (GetWorld()->GetTimerManager().IsTimerActive(NotificationTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(NotificationTimerHandle);
		}
		
		// Set timer to hide the notification
		GetWorld()->GetTimerManager().SetTimer(
			NotificationTimerHandle,
			this,
			&UGameHUDWidget::HideNotification,
			Duration,
			false);
	}
}

void UGameHUDWidget::UpdateObjective(const FText& ObjectiveText)
{
	if (CurrentObjectiveText)
	{
		CurrentObjectiveText->SetText(ObjectiveText);
	}
}

void UGameHUDWidget::HideNotification()
{
	if (NotificationText)
	{
		NotificationText->SetVisibility(ESlateVisibility::Hidden);
	}
}
