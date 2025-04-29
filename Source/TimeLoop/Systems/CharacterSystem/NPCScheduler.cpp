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

#include "NPCScheduler.h"
#include "Systems/TimeSystem/TimeManager.h"
#include "Systems/TimeSystem/TimeLoopSaveGame.h"
#include "NPCCharacter.h"  // This will need to be created later

UNPCScheduler::UNPCScheduler()
{
    // Default initialization
}

void UNPCScheduler::Initialize(UTimeManager* InTimeManager)
{
    // Store reference to time manager
    TimeManager = InTimeManager;
    
    // Register for time events
    if (TimeManager)
    {
        TimeManager->OnHourChanged.AddDynamic(this, &UNPCScheduler::OnHourChanged);
        TimeManager->OnDayReset.AddDynamic(this, &UNPCScheduler::OnDayReset);
        
        // Initialize with current hour
        int32 CurrentHour = TimeManager->GetCurrentHour();
        OnHourChanged(CurrentHour);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Initialized"));
}

void UNPCScheduler::RegisterNPC(FName NPCId, ANPCCharacter* NPCCharacter)
{
    // Add NPC to our tracking collections
    NPCCharacters.Add(NPCId, NPCCharacter);
    
    // Initialize relationship if not already present
    if (!RelationshipValues.Contains(NPCId))
    {
        RelationshipValues.Add(NPCId, 0.0f); // Start with neutral relationship
    }
    
    // Initialize state if not already present
    if (!NPCStates.Contains(NPCId))
    {
        NPCStates.Add(NPCId, FNPCState());
    }
    
    // Initialize with current hour if we have a time manager
    if (TimeManager)
    {
        UpdateNPCForTime(NPCId, TimeManager->GetCurrentHour());
    }
    
    UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Registered NPC %s"), *NPCId.ToString());
}

void UNPCScheduler::ResetAllNPCs()
{
    // Reset interaction flags for all NPCs
    for (auto& NPCState : NPCStates)
    {
        NPCState.Value.bInteractedToday = false;
    }
    
    // Update all NPCs to their current schedule positions
    if (TimeManager)
    {
        int32 CurrentHour = TimeManager->GetCurrentHour();
        for (auto& NPCPair : NPCStates)
        {
            UpdateNPCForTime(NPCPair.Key, CurrentHour);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Reset all NPCs for new day"));
}

FName UNPCScheduler::GetNPCLocation(FName NPCId) const
{
    if (NPCStates.Contains(NPCId))
    {
        return NPCStates[NPCId].CurrentLocation;
    }
    return NAME_None;
}

FName UNPCScheduler::GetNPCActivity(FName NPCId) const
{
    if (NPCStates.Contains(NPCId))
    {
        return NPCStates[NPCId].CurrentActivity;
    }
    return NAME_None;
}

ENPCMood UNPCScheduler::GetNPCMood(FName NPCId) const
{
    if (NPCStates.Contains(NPCId))
    {
        return NPCStates[NPCId].CurrentMood;
    }
    return ENPCMood::Neutral;
}

void UNPCScheduler::SetNPCMood(FName NPCId, ENPCMood NewMood)
{
    if (NPCStates.Contains(NPCId))
    {
        NPCStates[NPCId].CurrentMood = NewMood;
        
        // Update the NPC character if available
        if (NPCCharacters.Contains(NPCId) && NPCCharacters[NPCId] != nullptr)
        {
            // NPCCharacters[NPCId]->UpdateMood(NewMood); // Will implement in NPCCharacter class
        }
    }
}

float UNPCScheduler::GetRelationshipValue(FName NPCId) const
{
    if (RelationshipValues.Contains(NPCId))
    {
        return RelationshipValues[NPCId];
    }
    return 0.0f;  // Default to neutral
}

void UNPCScheduler::ChangeRelationship(FName NPCId, float Delta)
{
    // Initialize if not present
    if (!RelationshipValues.Contains(NPCId))
    {
        RelationshipValues.Add(NPCId, 0.0f);
    }
    
    // Update value, clamped to a reasonable range
    float NewValue = RelationshipValues[NPCId] + Delta;
    NewValue = FMath::Clamp(NewValue, -100.0f, 100.0f);
    RelationshipValues[NPCId] = NewValue;
    
    // Update mood based on relationship change
    if (Delta >= 5.0f)
    {
        SetNPCMood(NPCId, ENPCMood::Happy);
    }
    else if (Delta <= -5.0f)
    {
        SetNPCMood(NPCId, ENPCMood::Angry);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Relationship with %s changed by %.1f to %.1f"), 
        *NPCId.ToString(), Delta, NewValue);
}

void UNPCScheduler::SetNPCInteracted(FName NPCId)
{
    if (NPCStates.Contains(NPCId))
    {
        NPCStates[NPCId].bInteractedToday = true;
        
        UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Marked interaction with NPC %s"), 
            *NPCId.ToString());
    }
}

bool UNPCScheduler::HasInteractedToday(FName NPCId) const
{
    if (NPCStates.Contains(NPCId))
    {
        return NPCStates[NPCId].bInteractedToday;
    }
    return false;
}

TArray<FName> UNPCScheduler::GetNPCsAtLocation(FName LocationId) const
{
    TArray<FName> NPCsAtLocation;
    
    for (const auto& Pair : NPCStates)
    {
        if (Pair.Value.CurrentLocation == LocationId)
        {
            NPCsAtLocation.Add(Pair.Key);
        }
    }
    
    return NPCsAtLocation;
}

void UNPCScheduler::SaveNPCRelationships(UTimeLoopSaveGame* SaveGame)
{
    if (SaveGame)
    {
        // Copy our relationship values to the save game
        SaveGame->NPCRelationships = RelationshipValues;
        
        UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Saved %d NPC relationships"), 
            RelationshipValues.Num());
    }
}

void UNPCScheduler::LoadNPCRelationships(UTimeLoopSaveGame* SaveGame)
{
    if (SaveGame)
    {
        // Load relationship values from save game
        RelationshipValues = SaveGame->NPCRelationships;
        
        UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Loaded %d NPC relationships"), 
            RelationshipValues.Num());
    }
}

void UNPCScheduler::OnHourChanged(int32 NewHour)
{
    // Update all NPCs for the new hour
    for (auto& NPCPair : NPCStates)
    {
        UpdateNPCForTime(NPCPair.Key, NewHour);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("NPC Scheduler: Updated all NPCs for hour %d"), NewHour);
}

void UNPCScheduler::OnDayReset()
{
    // Reset NPCs for the new day
    ResetAllNPCs();
}

void UNPCScheduler::UpdateNPCForTime(FName NPCId, int32 CurrentHour)
{
    // Make sure the NPC exists
    if (!NPCStates.Contains(NPCId) || !NPCSchedules.Contains(NPCId))
    {
        return;
    }
    
    // Get the NPC's schedule
    const FNPCSchedule& Schedule = NPCSchedules[NPCId];
    
    // Find the appropriate schedule entry for this hour
    FScheduleEntry ScheduleEntry = FindScheduleEntryForHour(Schedule, CurrentHour);
    
    // Update the NPC's state
    FNPCState& State = NPCStates[NPCId];
    State.CurrentLocation = ScheduleEntry.LocationId;
    State.CurrentActivity = ScheduleEntry.ActivityId;
    
    // Update the NPC character if available
    if (NPCCharacters.Contains(NPCId) && NPCCharacters[NPCId] != nullptr)
    {
        // Will implement in NPCCharacter class:
        // NPCCharacters[NPCId]->UpdateSchedule(ScheduleEntry.LocationId, ScheduleEntry.ActivityId);
    }
}

FScheduleEntry UNPCScheduler::FindScheduleEntryForHour(const FNPCSchedule& Schedule, int32 Hour)
{
    FScheduleEntry BestEntry;
    
    // Find the latest entry that starts before or at the current hour
    int32 BestStartHour = -1;
    
    for (const FScheduleEntry& Entry : Schedule.Entries)
    {
        if (Entry.StartHour <= Hour && Entry.StartHour > BestStartHour)
        {
            BestStartHour = Entry.StartHour;
            BestEntry = Entry;
        }
    }
    
    // If no entry found (shouldn't happen with proper schedules), return default
    if (BestStartHour == -1 && Schedule.Entries.Num() > 0)
    {
        // Default to first entry
        BestEntry = Schedule.Entries[0];
    }
    
    return BestEntry;
}
