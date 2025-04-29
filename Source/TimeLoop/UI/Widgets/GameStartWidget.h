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
 * Widget for displaying game start sequence
 * Shows introduction and sets up the beginning of the time loop
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
	UFUNCTION(BlueprintCallable, Category = "UI|Intro")
	void StartIntroSequence();
	
	// Skip the intro sequence
	UFUNCTION(BlueprintCallable, Category = "UI|Intro")
	void SkipIntroSequence();
	
	// Delegate for when the intro sequence is complete
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIntroCompleteDelegate);
	UPROPERTY(BlueprintAssignable, Category = "UI|Intro")
	FOnIntroCompleteDelegate OnIntroComplete;
	
protected:
	// The container for intro text
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* IntroContainer;
	
	// The intro text blocks
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IntroText1;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IntroText2;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IntroText3;
	
	// The skip button
	UPROPERTY(meta = (BindWidget))
	class UButton* SkipButton;
	
	// The background image
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;
	
	// The music to play during intro
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* IntroMusic;
	
	// Whether the intro is currently playing
	bool bIsPlaying;
	
	// Audio component for playing music
	UPROPERTY()
	class UAudioComponent* MusicComponent;
	
	// Timer handles for intro sequence
	FTimerHandle IntroText1TimerHandle;
	FTimerHandle IntroText2TimerHandle;
	FTimerHandle IntroText3TimerHandle;
	FTimerHandle IntroCompleteTimerHandle;
	
	// Called when the skip button is clicked
	UFUNCTION()
	void OnSkipButtonClicked();
	
	// Show intro text 1
	UFUNCTION()
	void ShowIntroText1();
	
	// Show intro text 2
	UFUNCTION()
	void ShowIntroText2();
	
	// Show intro text 3
	UFUNCTION()
	void ShowIntroText3();
	
	// Complete the intro sequence
	UFUNCTION()
	void CompleteIntroSequence();
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Called when the widget is destroyed
	virtual void NativeDestruct() override;
};
