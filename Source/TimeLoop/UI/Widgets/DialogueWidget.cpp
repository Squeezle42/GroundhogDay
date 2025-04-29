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

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
	ClearDialogueOptions();
	
	if (!DialogueOptionsContainer || !DialogueOptionWidgetClass)
	{
		return;
	}
	
	for (const FDialogueOption& Option : Options)
	{
		UUserWidget* OptionWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), DialogueOptionWidgetClass);
		if (OptionWidget)
		{
			// Find the text block and button in the option widget
			UTextBlock* OptionText = Cast<UTextBlock>(OptionWidget->GetWidgetFromName(TEXT("OptionText")));
			UButton* OptionButton = Cast<UButton>(OptionWidget->GetWidgetFromName(TEXT("OptionButton")));
			
			if (OptionText)
			{
				OptionText->SetText(Option.OptionText);
			}
			
			if (OptionButton)
			{
				// Store the option ID with the button
				OptionButton->SetVisibility(ESlateVisibility::Visible);
				
				// Bind the click event
				FScriptDelegate ButtonDelegate;
				ButtonDelegate.BindUFunction(this, FName("OnDialogueOptionClicked"), Option.OptionID);
				OptionButton->OnClicked.Add(ButtonDelegate);
			}
			
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
	OnDialogueClosed.Broadcast();
	RemoveFromParent();
}

void UDialogueWidget::OnCloseButtonClicked()
{
	CloseDialogue();
}

void UDialogueWidget::OnDialogueOptionClicked(FName OptionID)
{
	OnDialogueOptionSelected.Broadcast(OptionID);
}
