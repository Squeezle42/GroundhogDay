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
#include "GameStartWidget.generated.h"

/**
 * Widget for displaying the game introduction sequence
 * Shows opening narrative and transitions into gameplay
 */
UCLASS()
class TIMELOOP_API UGameStartWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UGameStartWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
	// Start playing the intro sequence
	UFUNCTION(BlueprintCallable, Category = "UI|GameStart")
	void StartIntroSequence();
	
	// Skip the intro sequence
	UFUNCTION(BlueprintCallable, Category = "UI|GameStart")
	void SkipIntroSequence();
	
	// Complete the intro sequence
	UFUNCTION(BlueprintCallable, Category = "UI|GameStart")
	void CompleteIntroSequence();
	
	// Delegate for when the intro sequence is complete
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIntroSequenceCompleteDelegate);
	UPROPERTY(BlueprintAssignable, Category = "UI|GameStart")
	FOnIntroSequenceCompleteDelegate OnIntroSequenceComplete;
	
protected:
	// The container for intro text
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* IntroTextContainer;
	
	// The alarm clock text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AlarmClockText;
	
	// The intro narration text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NarrationText;
	
	// The skip button
	UPROPERTY(meta = (BindWidget))
	class UButton* SkipButton;
	
	// The continue button
	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;
	
	// The background image
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;
	
	// The radio music to play during intro
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* IntroMusic;
	
	// The alarm clock sound
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* AlarmSound;
	
	// Whether the intro is currently playing
	bool bIsPlaying;
	
	// Current intro step
	int32 CurrentIntroStep;
	
	// Timer handle for intro sequence
	FTimerHandle IntroSequenceTimerHandle;
	
	// Audio component for playing music
	UPROPERTY()
	class UAudioComponent* MusicComponent;
	
	// Called when the skip button is clicked
	UFUNCTION()
	void OnSkipButtonClicked();
	
	// Called when the continue button is clicked
	UFUNCTION()
	void OnContinueButtonClicked();
	
	// Advance to the next intro step
	void AdvanceIntroStep();
	
	// Set up the intro text sequences
	void SetupIntroSequences();
	
	// Array of intro text sequences
	TArray<FText> IntroSequences;
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Called when the widget is destroyed
	virtual void NativeDestruct() override;
};
