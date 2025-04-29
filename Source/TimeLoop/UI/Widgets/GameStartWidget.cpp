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
{
	// Load intro music
	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultIntroMusic(TEXT("/Game/Audio/Music/IntroMusic"));
	if (DefaultIntroMusic.Succeeded())
	{
		IntroMusic = DefaultIntroMusic.Object;
	}
}

void UGameStartWidget::InitializeWidget()
{
	Super::InitializeWidget();
	
	// Set the intro texts
	if (IntroText1)
	{
		IntroText1->SetText(FText::FromString("February 2nd, Groundhog Day..."));
		IntroText1->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (IntroText2)
	{
		IntroText2->SetText(FText::FromString("You wake up to the same day, over and over again..."));
		IntroText2->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (IntroText3)
	{
		IntroText3->SetText(FText::FromString("Can you break the time loop and escape?"));
		IntroText3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGameStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Bind skip button click event
	if (SkipButton)
	{
		SkipButton->OnClicked.AddDynamic(this, &UGameStartWidget::OnSkipButtonClicked);
		SkipButton->SetVisibility(ESlateVisibility::Hidden); // Initially hidden
	}
}

void UGameStartWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	// Stop the music if it's playing
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}
	
	// Clear any timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(IntroText1TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(IntroText2TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(IntroText3TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(IntroCompleteTimerHandle);
	}
}

void UGameStartWidget::StartIntroSequence()
{
	if (bIsPlaying)
	{
		return;
	}
	
	bIsPlaying = true;
	
	// Show the skip button
	if (SkipButton)
	{
		SkipButton->SetVisibility(ESlateVisibility::Visible);
	}
	
	// Play the intro music
	if (IntroMusic)
	{
		MusicComponent = UGameplayStatics::CreateSound2D(this, IntroMusic);
		if (MusicComponent)
		{
			MusicComponent->Play();
		}
	}
	
	// Set timers for the intro sequence
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(IntroText1TimerHandle, this, &UGameStartWidget::ShowIntroText1, 2.0f, false);
		GetWorld()->GetTimerManager().SetTimer(IntroText2TimerHandle, this, &UGameStartWidget::ShowIntroText2, 6.0f, false);
		GetWorld()->GetTimerManager().SetTimer(IntroText3TimerHandle, this, &UGameStartWidget::ShowIntroText3, 10.0f, false);
		GetWorld()->GetTimerManager().SetTimer(IntroCompleteTimerHandle, this, &UGameStartWidget::CompleteIntroSequence, 14.0f, false);
	}
}

void UGameStartWidget::SkipIntroSequence()
{
	// Clear any timers
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(IntroText1TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(IntroText2TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(IntroText3TimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(IntroCompleteTimerHandle);
	}
	
	// Show all text at once before completing
	if (IntroText1)
	{
		IntroText1->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (IntroText2)
	{
		IntroText2->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (IntroText3)
	{
		IntroText3->SetVisibility(ESlateVisibility::Visible);
	}
	
	// Complete the sequence after a short delay
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(IntroCompleteTimerHandle, this, &UGameStartWidget::CompleteIntroSequence, 1.0f, false);
	}
}

void UGameStartWidget::OnSkipButtonClicked()
{
	SkipIntroSequence();
}

void UGameStartWidget::ShowIntroText1()
{
	if (IntroText1)
	{
		IntroText1->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameStartWidget::ShowIntroText2()
{
	if (IntroText2)
	{
		IntroText2->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameStartWidget::ShowIntroText3()
{
	if (IntroText3)
	{
		IntroText3->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameStartWidget::CompleteIntroSequence()
{
	bIsPlaying = false;
	
	// Trigger the OnIntroComplete delegate
	OnIntroComplete.Broadcast();
	
	// Remove the widget from the viewport
	RemoveFromParent();
}
