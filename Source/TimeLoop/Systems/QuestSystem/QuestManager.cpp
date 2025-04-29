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

#include "QuestManager.h"
#include "Systems/TimeSystem/TimeLoopSaveGame.h"

UQuestManager::UQuestManager()
{
    // Default initialization
}

void UQuestManager::Initialize()
{
    UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Initialized"));
}

void UQuestManager::AddQuest(const FQuest& Quest)
{
    // Add to quest map
    Quests.Add(Quest.QuestId, Quest);
    
    UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Added quest %s"), *Quest.QuestId.ToString());
}

void UQuestManager::UpdateQuestState(FName QuestId, EQuestState NewState)
{
    // Make sure the quest exists
    if (!Quests.Contains(QuestId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Cannot update state for non-existent quest %s"), 
            *QuestId.ToString());
        return;
    }
    
    // Update the quest state
    FQuest& Quest = Quests[QuestId];
    EQuestState OldState = Quest.State;
    Quest.State = NewState;
    
    UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Quest %s state changed from %d to %d"), 
        *QuestId.ToString(), static_cast<int32>(OldState), static_cast<int32>(NewState));
}

void UQuestManager::CompleteObjective(FName QuestId, FName ObjectiveId)
{
    // Make sure the quest exists
    if (!Quests.Contains(QuestId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Cannot complete objective for non-existent quest %s"), 
            *QuestId.ToString());
        return;
    }
    
    // Get the quest
    FQuest& Quest = Quests[QuestId];
    
    // Find the objective
    bool bFoundObjective = false;
    for (FQuestObjective& Objective : Quest.Objectives)
    {
        if (Objective.ObjectiveId == ObjectiveId)
        {
            // Mark it as completed
            Objective.bCompleted = true;
            bFoundObjective = true;
            
            UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Completed objective %s for quest %s"), 
                *ObjectiveId.ToString(), *QuestId.ToString());
            break;
        }
    }
    
    if (!bFoundObjective)
    {
        UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Could not find objective %s in quest %s"), 
            *ObjectiveId.ToString(), *QuestId.ToString());
    }
    
    // Check if all objectives are now completed
    if (bFoundObjective && AreAllObjectivesCompleted(Quest))
    {
        // Auto-complete quest
        UpdateQuestState(QuestId, EQuestState::Completed);
        
        UE_LOG(LogTemp, Warning, TEXT("Quest Manager: All objectives completed, quest %s marked as completed"), 
            *QuestId.ToString());
    }
}

bool UQuestManager::IsObjectiveCompleted(FName QuestId, FName ObjectiveId) const
{
    // Make sure the quest exists
    if (!Quests.Contains(QuestId))
    {
        return false;
    }
    
    // Get the quest
    const FQuest& Quest = Quests[QuestId];
    
    // Find the objective
    for (const FQuestObjective& Objective : Quest.Objectives)
    {
        if (Objective.ObjectiveId == ObjectiveId)
        {
            return Objective.bCompleted;
        }
    }
    
    return false;
}

FQuest UQuestManager::GetQuest(FName QuestId) const
{
    // Return the quest if it exists
    if (Quests.Contains(QuestId))
    {
        return Quests[QuestId];
    }
    
    // Return an empty quest if not found
    return FQuest();
}

TArray<FQuest> UQuestManager::GetActiveQuests() const
{
    TArray<FQuest> ActiveQuests;
    
    for (const auto& Pair : Quests)
    {
        if (Pair.Value.State == EQuestState::InProgress)
        {
            ActiveQuests.Add(Pair.Value);
        }
    }
    
    return ActiveQuests;
}

TArray<FQuest> UQuestManager::GetCompletedQuests() const
{
    TArray<FQuest> CompletedQuests;
    
    for (const auto& Pair : Quests)
    {
        if (Pair.Value.State == EQuestState::Completed)
        {
            CompletedQuests.Add(Pair.Value);
        }
    }
    
    return CompletedQuests;
}

void UQuestManager::ResetQuestsForNewDay()
{
    // Reset non-persistent quests to their initial state
    for (auto& Pair : Quests)
    {
        FQuest& Quest = Pair.Value;
        
        // Skip quests that persist across loops
        if (Quest.bPersistAcrossLoops)
        {
            continue;
        }
        
        // Reset quest state if it's in progress
        if (Quest.State == EQuestState::InProgress)
        {
            Quest.State = EQuestState::Available;
            
            // Reset all objectives
            for (FQuestObjective& Objective : Quest.Objectives)
            {
                Objective.bCompleted = false;
            }
            
            UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Reset quest %s for new day"), 
                *Quest.QuestId.ToString());
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Reset quests for new day"));
}

void UQuestManager::SetKnowledgeFlag(FName FlagName, bool bValue)
{
    // Set or update the knowledge flag
    KnowledgeFlags.Add(FlagName, bValue);
    
    UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Knowledge flag %s set to %s"), 
        *FlagName.ToString(), bValue ? TEXT("true") : TEXT("false"));
}

bool UQuestManager::HasKnowledgeFlag(FName FlagName) const
{
    if (KnowledgeFlags.Contains(FlagName))
    {
        return KnowledgeFlags[FlagName];
    }
    return false;
}

void UQuestManager::SavePlayerKnowledge(UTimeLoopSaveGame* SaveGame)
{
    if (SaveGame)
    {
        // Copy knowledge flags to save game
        SaveGame->KnowledgeFlags = KnowledgeFlags;
        
        // Copy persistent quest progress
        SaveGame->PersistentQuestProgress = PersistentProgress;
        
        UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Saved %d knowledge flags and %d progress values"), 
            KnowledgeFlags.Num(), PersistentProgress.Num());
    }
}

void UQuestManager::LoadPlayerKnowledge(UTimeLoopSaveGame* SaveGame)
{
    if (SaveGame)
    {
        // Load knowledge flags from save game
        KnowledgeFlags = SaveGame->KnowledgeFlags;
        
        // Load persistent quest progress
        PersistentProgress = SaveGame->PersistentQuestProgress;
        
        UE_LOG(LogTemp, Warning, TEXT("Quest Manager: Loaded %d knowledge flags and %d progress values"), 
            KnowledgeFlags.Num(), PersistentProgress.Num());
    }
}

bool UQuestManager::AreAllObjectivesCompleted(const FQuest& Quest) const
{
    // Check if all objectives are completed
    for (const FQuestObjective& Objective : Quest.Objectives)
    {
        if (!Objective.bCompleted)
        {
            return false;
        }
    }
    
    // All objectives are completed
    return true;
}
