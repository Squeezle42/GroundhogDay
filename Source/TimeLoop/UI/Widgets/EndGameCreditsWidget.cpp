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

#include "EndGameCreditsWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

UEndGameCreditsWidget::UEndGameCreditsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsPlaying(false)
{
	// Load credits music
	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultCreditsMusic(TEXT("/Game/Audio/Music/RocketMan_ChrisHadfield"));
	if (DefaultCreditsMusic.Succeeded())
	{
		CreditsMusic = DefaultCreditsMusic.Object;
	}
}

void UEndGameCreditsWidget::InitializeWidget()
{
	Super::InitializeWidget();
	
	// Set the Bill Murray thank you text
	if (BillMurrayText)
	{
		BillMurrayText->SetText(FText::FromString("Special Thanks to Bill Murray\nFor the timeless inspiration from \"Groundhog Day\"\nWithout whom this game would not exist"));
	}
	
	// Set the Chris Hadfield text
	if (ChrisHadfieldText)
	{
		ChrisHadfieldText->SetText(FText::FromString("Music performed by\nCommander Chris Hadfield"));
	}
	
	// Set the song credit text
	if (SongCreditText)
	{
		SongCreditText->SetText(FText::FromString("\"Rocket Man\"\nOriginal by Elton John\nLyrics by Bernie Taupin"));
	}
}

void UEndGameCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Bind close button click event
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UEndGameCreditsWidget::OnCloseButtonClicked);
	}
}

void UEndGameCreditsWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	// Stop the music if it's playing
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}
}

void UEndGameCreditsWidget::StartCreditsSequence()
{
	if (bIsPlaying)
	{
		return;
	}
	
	bIsPlaying = true;
	
	// Play the credits music
	if (CreditsMusic)
	{
		MusicComponent = UGameplayStatics::CreateSound2D(this, CreditsMusic);
		if (MusicComponent)
		{
			MusicComponent->Play();
		}
	}
	
	// Show the credits text with animation
	// In a real implementation, you would have a more sophisticated animation
	// system, but for simplicity we'll just make them visible
	if (CreditsContainer)
	{
		CreditsContainer->SetVisibility(ESlateVisibility::Visible);
	}
	
	// Play any animations (these would be set up in the UMG editor)
	if (ShowAnimation)
	{
		PlayAnimation(ShowAnimation);
	}
}

void UEndGameCreditsWidget::OnCloseButtonClicked()
{
	// Stop the music
	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent = nullptr;
	}
	
	bIsPlaying = false;
	
	// Remove the widget from the viewport
	RemoveFromParent();
	
	// Return to the main menu or restart the game
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
