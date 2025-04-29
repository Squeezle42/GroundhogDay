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
#include "NPCScheduler.generated.h"

class UTimeManager;
class UTimeLoopSaveGame;
class ANPCCharacter;

/**
 * FScheduleEntry - Represents a single entry in an NPC's schedule
 */
USTRUCT(BlueprintType)
struct FScheduleEntry
{
    GENERATED_BODY()

    // The hour at which this schedule entry begins
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Schedule")
    int32 StartHour;

    // The location where the NPC should be during this time
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Schedule")
    FName LocationId;

    // The activity the NPC performs at this location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Schedule")
    FName ActivityId;

    // Default constructor
    FScheduleEntry()
        : StartHour(0)
        , LocationId(NAME_None)
        , ActivityId(NAME_None)
    {
    }

    // Constructor with all parameters
    FScheduleEntry(int32 InStartHour, FName InLocationId, FName InActivityId)
        : StartHour(InStartHour)
        , LocationId(InLocationId)
        , ActivityId(InActivityId)
    {
    }
};

/**
 * FNPCSchedule - Represents a complete daily schedule for an NPC
 */
USTRUCT(BlueprintType)
struct FNPCSchedule
{
    GENERATED_BODY()

    // All schedule entries for the day, sorted by start hour
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Schedule")
    TArray<FScheduleEntry> Entries;
};

/**
 * ENPCMood - Represents the current emotional state of an NPC
 */
UENUM(BlueprintType)
enum class ENPCMood : uint8
{
    Neutral UMETA(DisplayName = "Neutral"),
    Happy UMETA(DisplayName = "Happy"),
    Sad UMETA(DisplayName = "Sad"),
    Angry UMETA(DisplayName = "Angry"),
    Worried UMETA(DisplayName = "Worried"),
    Excited UMETA(DisplayName = "Excited")
};

/**
 * FNPCState - Represents the current state of an NPC
 */
USTRUCT(BlueprintType)
struct FNPCState
{
    GENERATED_BODY()

    // The NPC's current location
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC State")
    FName CurrentLocation;

    // The NPC's current activity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC State")
    FName CurrentActivity;

    // The NPC's current mood
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC State")
    ENPCMood CurrentMood;

    // Whether the player has interacted with this NPC today
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC State")
    bool bInteractedToday;

    // Default constructor
    FNPCState()
        : CurrentLocation(NAME_None)
        , CurrentActivity(NAME_None)
        , CurrentMood(ENPCMood::Neutral)
        , bInteractedToday(false)
    {
    }
};

/**
 * UNPCScheduler - Manages NPC schedules and behaviors throughout the day
 */
UCLASS(Blueprintable)
class TIMELOOP_API UNPCScheduler : public UObject
{
    GENERATED_BODY()

public:
    UNPCScheduler();

    // Initialize the NPC scheduler with a reference to the time manager
    void Initialize(UTimeManager* InTimeManager);

    // Reset all NPCs to their initial state for a new day
    UFUNCTION(BlueprintCallable, Category = "NPC System")
    void ResetAllNPCs();

    // Register an NPC with the scheduler
    UFUNCTION(BlueprintCallable, Category = "NPC System")
    void RegisterNPC(FName NPCId, ANPCCharacter* NPCCharacter);

    // Get an NPC's current location
    UFUNCTION(BlueprintPure, Category = "NPC System")
    FName GetNPCLocation(FName NPCId) const;

    // Get an NPC's current activity
    UFUNCTION(BlueprintPure, Category = "NPC System")
    FName GetNPCActivity(FName NPCId) const;

    // Get an NPC's current mood
    UFUNCTION(BlueprintPure, Category = "NPC System")
    ENPCMood GetNPCMood(FName NPCId) const;

    // Set an NPC's mood
    UFUNCTION(BlueprintCallable, Category = "NPC System")
    void SetNPCMood(FName NPCId, ENPCMood NewMood);

    // Get the relationship value with an NPC (positive = good, negative = bad)
    UFUNCTION(BlueprintPure, Category = "NPC System")
    float GetRelationshipValue(FName NPCId) const;

    // Change the relationship value with an NPC
    UFUNCTION(BlueprintCallable, Category = "NPC System")
    void ChangeRelationship(FName NPCId, float Delta);

    // Mark that the player has interacted with an NPC today
    UFUNCTION(BlueprintCallable, Category = "NPC System")
    void SetNPCInteracted(FName NPCId);

    // Check if the player has interacted with an NPC today
    UFUNCTION(BlueprintPure, Category = "NPC System")
    bool HasInteractedToday(FName NPCId) const;

    // Get NPCs currently at a specific location
    UFUNCTION(BlueprintCallable, Category = "NPC System")
    TArray<FName> GetNPCsAtLocation(FName LocationId) const;

    // Save NPC relationships to save game
    void SaveNPCRelationships(UTimeLoopSaveGame* SaveGame);

    // Load NPC relationships from save game
    void LoadNPCRelationships(UTimeLoopSaveGame* SaveGame);

private:
    // Handle hour change event
    UFUNCTION()
    void OnHourChanged(int32 NewHour);

    // Handle day reset event
    UFUNCTION()
    void OnDayReset();

    // Update an NPC's state based on the current hour
    void UpdateNPCForTime(FName NPCId, int32 CurrentHour);

    // Find the appropriate schedule entry for an NPC at the given hour
    FScheduleEntry FindScheduleEntryForHour(const FNPCSchedule& Schedule, int32 Hour);

protected:
    // Reference to the time manager
    UPROPERTY()
    UTimeManager* TimeManager;

    // NPC schedules, mapped by NPC ID
    UPROPERTY()
    TMap<FName, FNPCSchedule> NPCSchedules;

    // Current NPC states, mapped by NPC ID
    UPROPERTY()
    TMap<FName, FNPCState> NPCStates;

    // NPC relationship values, mapped by NPC ID
    UPROPERTY()
    TMap<FName, float> RelationshipValues;

    // Map of NPC character references
    UPROPERTY()
    TMap<FName, ANPCCharacter*> NPCCharacters;
};
