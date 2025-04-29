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

#include "GameStartWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

UGameStartWidget::UGameStartWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsPlaying(false)
	, CurrentIntroStep(0)
{
	// Load intro music
	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultIntroMusic(TEXT("/Game/Audio/Music/MorningRadio"));
	if (DefaultIntroMusic.Succeeded())
	{
		IntroMusic = DefaultIntroMusic.Object;
	}
	
	// Load alarm sound
	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultAlarmSound(TEXT("/Game/Audio/SFX/AlarmClock"));
	if (DefaultAlarmSound.Succeeded())
	{
		AlarmSound = DefaultAlarmSound.Object;
	}
}

void UGameStartWidget::InitializeWidget()
{
	Super::InitializeWidget();
	
	// Set up intro sequences
	SetupIntroSequences();
	
	// Hide the continue button initially
	if (ContinueButton)
	{
		ContinueButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Bind button click events
	if (SkipButton)
	{
		SkipButton->OnClicked.AddDynamic(this, &UGameStartWidget::OnSkipButtonClicked);
	}
	
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UGameStartWidget::OnContinueButtonClicked);
	}
}

void UGameStartWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	// Stop any timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(IntroSequenceTimerHandle);
	}
	
	// Stop the music if it's playing
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}
}

void UGameStartWidget::SetupIntroSequences()
{
	// Initial sequence - clock display
	IntroSequences.Add(FText::FromString("6:00 AM"));
	
	// Introduction narrations
	IntroSequences.Add(FText::FromString("*BEEP* *BEEP* *BEEP*"));
	IntroSequences.Add(FText::FromString("You reach over and slap the alarm clock."));
	IntroSequences.Add(FText::FromString("Another day in Groundhog Hollow..."));
	IntroSequences.Add(FText::FromString("Wait, didn't you just live this exact same morning yesterday?"));
	IntroSequences.Add(FText::FromString("The radio plays the same song. The announcer says the same words."));
	IntroSequences.Add(FText::FromString("\"It's Groundhog Day, folks! The annual festival begins today!\""));
	IntroSequences.Add(FText::FromString("You have a strange feeling that something isn't right..."));
}

void UGameStartWidget::StartIntroSequence()
{
	if (bIsPlaying)
	{
		return;
	}
	
	bIsPlaying = true;
	CurrentIntroStep = 0;
	
	// Play the alarm sound
	if (AlarmSound)
	{
		UGameplayStatics::PlaySound2D(this, AlarmSound);
	}
	
	// Start the intro music after a short delay
	FTimerHandle MusicTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MusicTimerHandle, [this]()
	{
		if (IntroMusic)
		{
			MusicComponent = UGameplayStatics::CreateSound2D(this, IntroMusic);
			if (MusicComponent)
			{
				MusicComponent->Play();
			}
		}
	}, 3.0f, false);
	
	// Display the first intro step
	AdvanceIntroStep();
}

void UGameStartWidget::AdvanceIntroStep()
{
	if (CurrentIntroStep < IntroSequences.Num())
	{
		FText CurrentText = IntroSequences[CurrentIntroStep];
		
		// Display the text in the appropriate text block
		if (CurrentIntroStep == 0)
		{
			if (AlarmClockText)
			{
				AlarmClockText->SetText(CurrentText);
				AlarmClockText->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if (NarrationText)
			{
				NarrationText->SetText(CurrentText);
			}
		}
		
		// Advance to the next step after a delay
		float Delay = 3.0f;
		GetWorld()->GetTimerManager().SetTimer(IntroSequenceTimerHandle, this, &UGameStartWidget::AdvanceIntroStep, Delay, false);
		
		CurrentIntroStep++;
	}
	else
	{
		// End of the sequence, show the continue button
		if (ContinueButton)
		{
			ContinueButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UGameStartWidget::SkipIntroSequence()
{
	// Stop any timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(IntroSequenceTimerHandle);
	}
	
	// Stop the music
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}
	
	// Complete the intro sequence
	CompleteIntroSequence();
}

void UGameStartWidget::CompleteIntroSequence()
{
	bIsPlaying = false;
	
	// Broadcast that the intro sequence is complete
	OnIntroSequenceComplete.Broadcast();
	
	// Remove from viewport
	RemoveFromParent();
}

void UGameStartWidget::OnSkipButtonClicked()
{
	SkipIntroSequence();
}

void UGameStartWidget::OnContinueButtonClicked()
{
	CompleteIntroSequence();
}
