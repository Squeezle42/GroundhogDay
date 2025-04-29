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

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMainMenuWidget::InitializeWidget()
{
	Super::InitializeWidget();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Bind button click events
	if (NewGameButton)
	{
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	}
	
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinueClicked);
		
		// Disable the continue button if no save game exists
		bool bSaveGameExists = UGameplayStatics::DoesSaveGameExist(TEXT("TimeLoopSaveGame"), 0);
		ContinueButton->SetIsEnabled(bSaveGameExists);
	}
	
	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
	}
	
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
}

void UMainMenuWidget::OnNewGameClicked()
{
	// Start a new game
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainGameLevel"));
}

void UMainMenuWidget::OnContinueClicked()
{
	// Load the saved game and continue
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainGameLevel"));
	
	// Note: The actual loading will be handled by the GameMode when the level is loaded
}

void UMainMenuWidget::OnOptionsClicked()
{
	// Show options menu - this would be implemented in a real game
	// For now, just log a message
	UE_LOG(LogTemp, Display, TEXT("Options menu clicked - functionality not implemented yet"));
}

void UMainMenuWidget::OnQuitClicked()
{
	// Quit the game
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
	}
}
