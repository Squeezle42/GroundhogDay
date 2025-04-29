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
#include "DialogueWidget.generated.h"

/**
 * Dialogue option structure
 */
USTRUCT(BlueprintType)
struct FDialogueOption
{
	GENERATED_BODY()
	
	// The text to display for this option
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OptionText;
	
	// Unique identifier for this option
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName OptionID;
	
	// Constructor
	FDialogueOption()
		: OptionText(FText::GetEmpty())
		, OptionID(NAME_None)
	{}
	
	// Constructor with parameters
	FDialogueOption(FText InOptionText, FName InOptionID)
		: OptionText(InOptionText)
		, OptionID(InOptionID)
	{}
};

/**
 * Dialogue widget for the TimeLoop game
 * Handles displaying NPC dialogue and player responses
 */
UCLASS()
class TIMELOOP_API UDialogueWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UDialogueWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
	// Set the speaker name
	UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
	void SetSpeakerName(const FText& SpeakerName);
	
	// Set the dialogue text
	UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
	void SetDialogueText(const FText& DialogueText);
	
	// Set the available dialogue options
	UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
	void SetDialogueOptions(const TArray<FDialogueOption>& Options);
	
	// Clear all dialogue options
	UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
	void ClearDialogueOptions();
	
	// Close the dialogue widget
	UFUNCTION(BlueprintCallable, Category = "UI|Dialogue")
	void CloseDialogue();
	
	// Delegate for when a dialogue option is selected
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueOptionSelectedDelegate, FName, OptionID);
	UPROPERTY(BlueprintAssignable, Category = "UI|Dialogue")
	FOnDialogueOptionSelectedDelegate OnDialogueOptionSelected;
	
	// Delegate for when the dialogue is closed
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueClosedDelegate);
	UPROPERTY(BlueprintAssignable, Category = "UI|Dialogue")
	FOnDialogueClosedDelegate OnDialogueClosed;

protected:
	// The speaker name text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SpeakerNameText;
	
	// The dialogue text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DialogueText;
	
	// The container for dialogue options
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* DialogueOptionsContainer;
	
	// The close button
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;
	
	// The class to use for dialogue option buttons
	UPROPERTY(EditDefaultsOnly, Category = "UI|Dialogue")
	TSubclassOf<class UUserWidget> DialogueOptionWidgetClass;
	
	// Called when the close button is clicked
	UFUNCTION()
	void OnCloseButtonClicked();
	
	// Called when a dialogue option is clicked
	UFUNCTION()
	void OnDialogueOptionClicked(FName OptionID);
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
};
