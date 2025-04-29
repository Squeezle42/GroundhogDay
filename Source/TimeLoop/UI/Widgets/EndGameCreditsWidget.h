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
#include "EndGameCreditsWidget.generated.h"

/**
 * Widget for displaying end game credits
 * Shows special thanks and plays the end game music
 */
UCLASS()
class TIMELOOP_API UEndGameCreditsWidget : public UTimeLoopBaseWidget
{
	GENERATED_BODY()
	
public:
	UEndGameCreditsWidget(const FObjectInitializer& ObjectInitializer);
	
	// Initialize the widget with any required dependencies
	virtual void InitializeWidget() override;
	
	// Start playing the credits sequence
	UFUNCTION(BlueprintCallable, Category = "UI|Credits")
	void StartCreditsSequence();
	
protected:
	// The container for credits text
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CreditsContainer;
	
	// The Bill Murray thank you text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BillMurrayText;
	
	// The Chris Hadfield text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChrisHadfieldText;
	
	// The song credit text
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SongCreditText;
	
	// The close button
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;
	
	// The background image
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;
	
	// The music to play during credits
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* CreditsMusic;
	
	// Whether the credits are currently playing
	bool bIsPlaying;
	
	// Audio component for playing music
	UPROPERTY()
	class UAudioComponent* MusicComponent;
	
	// Called when the close button is clicked
	UFUNCTION()
	void OnCloseButtonClicked();
	
	// Called when the widget is constructed
	virtual void NativeConstruct() override;
	
	// Called when the widget is destroyed
	virtual void NativeDestruct() override;
};
