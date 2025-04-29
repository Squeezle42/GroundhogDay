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

#include "TimeLoopHUD.h"
#include "UI/Widgets/TimeLoopBaseWidget.h"
#include "UI/Widgets/GameHUDWidget.h"
#include "UI/Widgets/MainMenuWidget.h"
#include "UI/Widgets/InventoryWidget.h"
#include "UI/Widgets/DialogueWidget.h"
#include "UI/Widgets/EndGameCreditsWidget.h"
#include "Kismet/GameplayStatics.h"

ATimeLoopHUD::ATimeLoopHUD()
{
	// Set default classes for widgets
	static ConstructorHelpers::FClassFinder<UGameHUDWidget> GameHUDWidgetClassFinder(TEXT("/Game/UI/Widgets/WBP_GameHUD"));
	if (GameHUDWidgetClassFinder.Succeeded())
	{
		GameHUDWidgetClass = GameHUDWidgetClassFinder.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuWidgetClassFinder(TEXT("/Game/UI/Widgets/WBP_MainMenu"));
	if (MainMenuWidgetClassFinder.Succeeded())
	{
		MainMenuWidgetClass = MainMenuWidgetClassFinder.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UInventoryWidget> InventoryWidgetClassFinder(TEXT("/Game/UI/Widgets/WBP_Inventory"));
	if (InventoryWidgetClassFinder.Succeeded())
	{
		InventoryWidgetClass = InventoryWidgetClassFinder.Class;
	}
		static ConstructorHelpers::FClassFinder<UDialogueWidget> DialogueWidgetClassFinder(TEXT("/Game/UI/Widgets/WBP_Dialogue"));
	if (DialogueWidgetClassFinder.Succeeded())
	{
		DialogueWidgetClass = DialogueWidgetClassFinder.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UEndGameCreditsWidget> EndGameCreditsWidgetClassFinder(TEXT("/Game/UI/Widgets/WBP_EndGameCredits"));
	if (EndGameCreditsWidgetClassFinder.Succeeded())
	{
		EndGameCreditsWidgetClass = EndGameCreditsWidgetClassFinder.Class;
	}
}

void ATimeLoopHUD::BeginPlay()
{
	Super::BeginPlay();
	
	// Create the game HUD widget first (always visible)
	GameHUDWidget = CreateWidget<UGameHUDWidget>(GameHUDWidgetClass);
		// Create other widgets but don't add them to viewport yet
	MainMenuWidget = CreateWidget<UMainMenuWidget>(MainMenuWidgetClass, false);
	InventoryWidget = CreateWidget<UInventoryWidget>(InventoryWidgetClass, false);
	DialogueWidget = CreateWidget<UDialogueWidget>(DialogueWidgetClass, false);
	EndGameCreditsWidget = CreateWidget<UEndGameCreditsWidget>(EndGameCreditsWidgetClass, false);
}

void ATimeLoopHUD::DrawHUD()
{
	Super::DrawHUD();
	
	// Any per-frame HUD drawing can be done here
}

void ATimeLoopHUD::ToggleMainMenu()
{
	if (MainMenuWidget)
	{
		if (MainMenuWidget->IsInViewport())
		{
			MainMenuWidget->RemoveFromParent();
			
			// Unpause the game when closing the menu
			UGameplayStatics::SetGamePaused(GetWorld(), false);
		}
		else
		{
			MainMenuWidget->AddToViewport();
			
			// Pause the game when opening the menu
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			
			// Give input focus to the widget
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeUIOnly());
				PC->SetShowMouseCursor(true);
			}
		}
	}
}

void ATimeLoopHUD::ToggleInventory()
{
	if (InventoryWidget)
	{
		if (InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
			
			// Return to game input mode
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeGameOnly());
				PC->SetShowMouseCursor(false);
			}
		}
		else
		{
			InventoryWidget->AddToViewport();
			
			// Set UI input mode but don't pause
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeGameAndUI());
				PC->SetShowMouseCursor(true);
			}
		}
	}
}

void ATimeLoopHUD::ShowDialogueUI(bool bShow)
{
	if (DialogueWidget)
	{
		if (bShow && !DialogueWidget->IsInViewport())
		{
			DialogueWidget->AddToViewport();
			
			// Set UI input mode but don't pause
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeGameAndUI());
				PC->SetShowMouseCursor(true);
			}
		}
		else if (!bShow && DialogueWidget->IsInViewport())
		{
			DialogueWidget->RemoveFromParent();
			
			// Return to game input mode
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeGameOnly());
				PC->SetShowMouseCursor(false);
			}
		}
	}
}

void ATimeLoopHUD::ShowEndGameCredits()
{
	if (EndGameCreditsWidget)
	{
		// Hide any other UI elements first
		if (GameHUDWidget && GameHUDWidget->IsInViewport())
		{
			GameHUDWidget->RemoveFromParent();
		}
		
		if (InventoryWidget && InventoryWidget->IsInViewport())
		{
			InventoryWidget->RemoveFromParent();
		}
		
		if (DialogueWidget && DialogueWidget->IsInViewport())
		{
			DialogueWidget->RemoveFromParent();
		}
		
		// Show the end game credits
		if (!EndGameCreditsWidget->IsInViewport())
		{
			EndGameCreditsWidget->AddToViewport();
			EndGameCreditsWidget->StartCreditsSequence();
			
			// Set UI input mode
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeUIOnly());
				PC->SetShowMouseCursor(true);
			}
		}
	}
}

template<typename T>
T* ATimeLoopHUD::CreateWidget(TSubclassOf<T> WidgetClass, bool bAddToViewport)
{
	if (WidgetClass)
	{
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			T* Widget = CreateWidget<T>(PC, WidgetClass);
			if (Widget && bAddToViewport)
			{
				Widget->AddToViewport();
			}
			return Widget;
		}
	}
	
	return nullptr;
}
