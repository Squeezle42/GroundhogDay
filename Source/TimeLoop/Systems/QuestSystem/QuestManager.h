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
#include "UObject/NoExportTypes.h"
#include "QuestManager.generated.h"

class UTimeLoopSaveGame;

/**
 * EQuestState - Represents the possible states of a quest
 */
UENUM(BlueprintType)
enum class EQuestState : uint8
{
    Unavailable UMETA(DisplayName = "Unavailable"),
    Available UMETA(DisplayName = "Available"),
    InProgress UMETA(DisplayName = "In Progress"),
    Completed UMETA(DisplayName = "Completed"),
    Failed UMETA(DisplayName = "Failed")
};

/**
 * EQuestType - Represents the types of quests
 */
UENUM(BlueprintType)
enum class EQuestType : uint8
{
    MainStory UMETA(DisplayName = "Main Story"),
    Character UMETA(DisplayName = "Character"),
    Town UMETA(DisplayName = "Town"),
    Discovery UMETA(DisplayName = "Discovery"),
    TimeLoop UMETA(DisplayName = "Time Loop")
};

/**
 * FQuestObjective - Represents a single objective within a quest
 */
USTRUCT(BlueprintType)
struct FQuestObjective
{
    GENERATED_BODY()

    // Unique identifier for the objective
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName ObjectiveId;

    // Display text for the objective
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText Description;

    // Whether this objective has been completed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bCompleted;

    // Constructor
    FQuestObjective()
        : ObjectiveId(NAME_None)
        , bCompleted(false)
    {
    }

    // Constructor with parameters
    FQuestObjective(FName InObjectiveId, FText InDescription)
        : ObjectiveId(InObjectiveId)
        , Description(InDescription)
        , bCompleted(false)
    {
    }
};

/**
 * FQuest - Represents a quest in the game
 */
USTRUCT(BlueprintType)
struct FQuest
{
    GENERATED_BODY()

    // Unique identifier for the quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestId;

    // Display name of the quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText Title;

    // Quest description
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText Description;

    // Type of quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestType QuestType;

    // Current state of the quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestState State;

    // The objectives for this quest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FQuestObjective> Objectives;

    // The NPC that gave this quest, if any
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestGiver;

    // Whether this quest persists across time loops
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool bPersistAcrossLoops;

    // Constructor
    FQuest()
        : QuestId(NAME_None)
        , QuestType(EQuestType::Character)
        , State(EQuestState::Unavailable)
        , QuestGiver(NAME_None)
        , bPersistAcrossLoops(false)
    {
    }

    // Constructor with parameters
    FQuest(FName InQuestId, FText InTitle, FText InDescription, EQuestType InQuestType)
        : QuestId(InQuestId)
        , Title(InTitle)
        , Description(InDescription)
        , QuestType(InQuestType)
        , State(EQuestState::Unavailable)
        , QuestGiver(NAME_None)
        , bPersistAcrossLoops(false)
    {
    }
};

/**
 * UQuestManager - Manages quests and player knowledge
 */
UCLASS(Blueprintable)
class TIMELOOP_API UQuestManager : public UObject
{
    GENERATED_BODY()

public:
    UQuestManager();

    // Initialize the quest manager
    void Initialize();

    // Add a new quest to the system
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void AddQuest(const FQuest& Quest);

    // Update a quest's state
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void UpdateQuestState(FName QuestId, EQuestState NewState);

    // Complete an objective for a quest
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void CompleteObjective(FName QuestId, FName ObjectiveId);

    // Check if an objective is completed
    UFUNCTION(BlueprintPure, Category = "Quest System")
    bool IsObjectiveCompleted(FName QuestId, FName ObjectiveId) const;

    // Get a quest by ID
    UFUNCTION(BlueprintPure, Category = "Quest System")
    FQuest GetQuest(FName QuestId) const;

    // Get all active quests
    UFUNCTION(BlueprintPure, Category = "Quest System")
    TArray<FQuest> GetActiveQuests() const;

    // Get all completed quests
    UFUNCTION(BlueprintPure, Category = "Quest System")
    TArray<FQuest> GetCompletedQuests() const;

    // Reset quests for a new day/loop
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void ResetQuestsForNewDay();

    // Set a knowledge flag for the player
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void SetKnowledgeFlag(FName FlagName, bool bValue = true);

    // Check if the player has a knowledge flag
    UFUNCTION(BlueprintPure, Category = "Quest System")
    bool HasKnowledgeFlag(FName FlagName) const;

    // Save player knowledge to save game
    void SavePlayerKnowledge(UTimeLoopSaveGame* SaveGame);

    // Load player knowledge from save game
    void LoadPlayerKnowledge(UTimeLoopSaveGame* SaveGame);

protected:
    // All quests in the game
    UPROPERTY()
    TMap<FName, FQuest> Quests;

    // Player knowledge flags
    UPROPERTY()
    TMap<FName, bool> KnowledgeFlags;

    // Progress values that persist across loops
    UPROPERTY()
    TMap<FName, int32> PersistentProgress;

private:
    // Check if a quest's objectives are all complete
    bool AreAllObjectivesCompleted(const FQuest& Quest) const;
};
