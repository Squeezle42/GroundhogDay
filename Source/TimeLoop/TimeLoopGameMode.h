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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimeLoopGameMode.generated.h"

// Forward declarations
class UTimeManager;
class UNPCScheduler;
class UQuestManager;
class UDialogueManager;

/**
 * ATimeLoopGameMode - The main game mode for the Time Loop game
 * Manages the overall game state and coordinates between different systems
 */
UCLASS()
class TIMELOOP_API ATimeLoopGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATimeLoopGameMode();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	// Initiates a time loop reset (move to new day)
	UFUNCTION(BlueprintCallable, Category = "Time Loop")
	void ResetTimeLoop();
	
	// Save the current game state
	UFUNCTION(BlueprintCallable, Category = "Time Loop")
	void SaveGame();
	
	// Load the saved game state
	UFUNCTION(BlueprintCallable, Category = "Time Loop")
	void LoadGame();
	
	// Get the Time Manager
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	UTimeManager* GetTimeManager() const { return TimeManager; }
	
	// Get the NPC Scheduler
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	UNPCScheduler* GetNPCScheduler() const { return NPCScheduler; }
	
	// Get the Quest Manager
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	UQuestManager* GetQuestManager() const { return QuestManager; }
	
	// Get the Dialogue Manager
	UFUNCTION(BlueprintPure, Category = "Time Loop")
	UDialogueManager* GetDialogueManager() const { return DialogueManager; }
	
protected:
	// Initialize all game systems
	void InitializeGameSystems();
	
	// Reset all systems to their starting state
	void ResetAllSystems();
	
private:
	// The Time Manager handles game time progression
	UPROPERTY()
	UTimeManager* TimeManager;
	
	// The NPC Scheduler manages NPC schedules and behaviors
	UPROPERTY()
	UNPCScheduler* NPCScheduler;
	
	// The Quest Manager tracks quests and progress
	UPROPERTY()
	UQuestManager* QuestManager;
	
	// The Dialogue Manager handles dialogue interactions
	UPROPERTY()
	UDialogueManager* DialogueManager;
};
