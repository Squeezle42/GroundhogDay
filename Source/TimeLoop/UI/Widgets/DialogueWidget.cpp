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

#include "DialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "UI/TimeLoopHUD.h"

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Default dialogue option widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultDialogueOptionWidgetClass(TEXT("/Game/UI/Widgets/WBP_DialogueOption"));
	if (DefaultDialogueOptionWidgetClass.Succeeded())
	{
		DialogueOptionWidgetClass = DefaultDialogueOptionWidgetClass.Class;
	}
}

void UDialogueWidget::InitializeWidget()
{
	Super::InitializeWidget();
}

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Bind close button click event
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnCloseButtonClicked);
	}
}

void UDialogueWidget::SetSpeakerName(const FText& SpeakerName)
{
	if (SpeakerNameText)
	{
		SpeakerNameText->SetText(SpeakerName);
	}
}

void UDialogueWidget::SetDialogueText(const FText& DialogueText)
{
	if (this->DialogueText)
	{
		this->DialogueText->SetText(DialogueText);
	}
}

void UDialogueWidget::SetDialogueOptions(const TArray<FDialogueOption>& Options)
{
	// Clear existing options first
	ClearDialogueOptions();
	
	if (!DialogueOptionsContainer || !DialogueOptionWidgetClass)
	{
		return;
	}
	
	// Create widgets for each option
	for (const FDialogueOption& Option : Options)
	{
		UUserWidget* OptionWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), DialogueOptionWidgetClass);
		if (OptionWidget)
		{
			// Set the option text
			UTextBlock* OptionText = Cast<UTextBlock>(OptionWidget->GetWidgetFromName(TEXT("OptionText")));
			if (OptionText)
			{
				OptionText->SetText(Option.OptionText);
			}
			
			// Bind the button click event
			UButton* OptionButton = Cast<UButton>(OptionWidget->GetWidgetFromName(TEXT("OptionButton")));
			if (OptionButton)
			{
				FName OptionID = Option.OptionID;
				OptionButton->OnClicked.AddDynamic(this, &UDialogueWidget::OnDialogueOptionClicked);
				OptionButton->SetUserFocusDelegate(FGetUserFocus::CreateLambda([OptionID]() { return OptionID; }));
			}
			
			// Add the option to the container
			DialogueOptionsContainer->AddChild(OptionWidget);
		}
	}
}

void UDialogueWidget::ClearDialogueOptions()
{
	if (DialogueOptionsContainer)
	{
		DialogueOptionsContainer->ClearChildren();
	}
}

void UDialogueWidget::CloseDialogue()
{
	// Trigger the OnDialogueClosed delegate
	OnDialogueClosed.Broadcast();
	
	// Get the HUD and hide the dialogue UI
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		ATimeLoopHUD* HUD = Cast<ATimeLoopHUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->ShowDialogueUI(false);
		}
	}
}

void UDialogueWidget::OnCloseButtonClicked()
{
	CloseDialogue();
}

void UDialogueWidget::OnDialogueOptionClicked(FName OptionID)
{
	// Trigger the OnDialogueOptionSelected delegate
	OnDialogueOptionSelected.Broadcast(OptionID);
}
