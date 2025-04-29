// Copyright (C) 2025 Time Loop Game Development Team
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

#include "TimeLoopGameMode.h"
#include "Systems/TimeSystem/TimeManager.h"
#include "Systems/CharacterSystem/NPCScheduler.h"
#include "Systems/QuestSystem/QuestManager.h"
#include "Systems/DialogueSystem/DialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/TimeSystem/TimeLoopSaveGame.h"
#include "UI/TimeLoopHUD.h"

ATimeLoopGameMode::ATimeLoopGameMode()
{
	// Set this game mode to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;
	
	// Set default classes
	HUDClass = ATimeLoopHUD::StaticClass();
}

void ATimeLoopGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize all game systems
	InitializeGameSystems();
	
	// Start with the game intro sequence
	StartGameIntroSequence();
	
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Begin Play"));
}

void ATimeLoopGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Allow the time manager to update
	if (TimeManager)
	{
		TimeManager->UpdateTime(DeltaSeconds);
	}
}

void ATimeLoopGameMode::InitializeGameSystems()
{
	// Create the Time Manager
	TimeManager = NewObject<UTimeManager>(this);
	if (TimeManager)
	{
		TimeManager->Initialize();
	}
	
	// Create the NPC Scheduler
	NPCScheduler = NewObject<UNPCScheduler>(this);
	if (NPCScheduler && TimeManager)
	{
		NPCScheduler->Initialize(TimeManager);
	}
	
	// Create the Quest Manager
	QuestManager = NewObject<UQuestManager>(this);
	if (QuestManager)
	{
		QuestManager->Initialize();
	}
	
	// Create the Dialogue Manager
	DialogueManager = NewObject<UDialogueManager>(this);
	if (DialogueManager)
	{
		DialogueManager->Initialize();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Systems Initialized"));
}

void ATimeLoopGameMode::ResetTimeLoop()
{
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Initiating Time Loop Reset"));
	
	// Save the player's knowledge and important state before reset
	SaveGame();
	
	// Reset all systems to their starting state
	ResetAllSystems();
	
	// Load the saved player knowledge 
	LoadGame();
	
	// Broadcast that the time loop has been reset
	// Blueprint implementable event can be added here
}

void ATimeLoopGameMode::ResetAllSystems()
{
	// Reset each system to its initial state
	if (TimeManager)
	{
		TimeManager->ResetToMorning();
	}
	
	if (NPCScheduler)
	{
		NPCScheduler->ResetAllNPCs();
	}
	
	if (QuestManager)
	{
		QuestManager->ResetQuestsForNewDay();
	}
	
	if (DialogueManager)
	{
		DialogueManager->ResetForNewDay();
	}
}

void ATimeLoopGameMode::SaveGame()
{
	// Create a new save game instance
	UTimeLoopSaveGame* SaveGameInstance = Cast<UTimeLoopSaveGame>(UGameplayStatics::CreateSaveGameObject(UTimeLoopSaveGame::StaticClass()));
	
	if (SaveGameInstance)
	{
		// Store persistent data in the save game
		if (TimeManager)
		{
			SaveGameInstance->LoopCount = TimeManager->GetLoopCount();
		}
		
		// Save the player's knowledge flags
		if (QuestManager)
		{
			QuestManager->SavePlayerKnowledge(SaveGameInstance);
		}
		
		// Save NPC relationship data
		if (NPCScheduler)
		{
			NPCScheduler->SaveNPCRelationships(SaveGameInstance);
		}
		
		// Save the game to slot
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("TimeLoopSave"), 0);
		
		UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Game Saved"));
	}
}

void ATimeLoopGameMode::LoadGame()
{
	// Check if a save game exists
	if (UGameplayStatics::DoesSaveGameExist(TEXT("TimeLoopSave"), 0))
	{
		// Load the save game
		UTimeLoopSaveGame* SaveGameInstance = Cast<UTimeLoopSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TimeLoopSave"), 0));
		
		if (SaveGameInstance)
		{
			// Load persistent data from the save game
			if (TimeManager)
			{
				TimeManager->SetLoopCount(SaveGameInstance->LoopCount);
			}
			
			// Load the player's knowledge flags
			if (QuestManager)
			{
				QuestManager->LoadPlayerKnowledge(SaveGameInstance);
			}
			
			// Load NPC relationship data
			if (NPCScheduler)
			{
				NPCScheduler->LoadNPCRelationships(SaveGameInstance);
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Game Loaded"));
		}
	}
}

void ATimeLoopGameMode::StartGameIntroSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Starting Game Intro Sequence"));
	
	// Get the player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Time Loop Game Mode: Failed to get player controller"));
		return;
	}
	
	// Get the HUD
	ATimeLoopHUD* HUD = PC->GetHUD<ATimeLoopHUD>();
	if (HUD)
	{
		HUD->ShowGameStart();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Time Loop Game Mode: Failed to get TimeLoopHUD"));
	}
}

void ATimeLoopGameMode::SkipGameIntroSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Skipping Game Intro Sequence"));
	
	// Get the player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}
	
	// Get the HUD
	ATimeLoopHUD* HUD = PC->GetHUD<ATimeLoopHUD>();
	if (HUD)
	{
		HUD->SkipGameStart();
	}
}

void ATimeLoopGameMode::StartGameEndingSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Starting Game Ending Sequence"));
	
	// Get the player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}
	
	// Get the HUD
	ATimeLoopHUD* HUD = PC->GetHUD<ATimeLoopHUD>();
	if (HUD)
	{
		// Show the ending credits with special thanks to Bill Murray and Chris Hadfield
		HUD->ShowEndGameCredits();
	}
}

void ATimeLoopGameMode::OnGameIntroSequenceComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("Time Loop Game Mode: Game Intro Sequence Complete"));
	
	// Start the actual gameplay
	// For our current implementation, we'll immediately jump to the ending
	// This will be replaced with actual gameplay in future versions
	
	// Get the player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		return;
	}
	
	// Get the HUD
	ATimeLoopHUD* HUD = PC->GetHUD<ATimeLoopHUD>();
	if (HUD)
	{
		// Show the game HUD
		// In a full game, we would start regular gameplay here
		// For now, we'll add a key binding to trigger the ending
	}
}
