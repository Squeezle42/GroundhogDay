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
#include "GameFramework/Character.h"
#include "Systems/CharacterSystem/NPCScheduler.h"
#include "NPCCharacter.generated.h"

class UTimeLoopGameMode;
class UNPCScheduler;
class UDialogueManager;

/**
 * ANPCCharacter - Base class for all NPCs in the game
 */
UCLASS()
class TIMELOOP_API ANPCCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ANPCCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Interact with this NPC
    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void Interact();

    // Start dialogue with this NPC
    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual bool StartDialogue();

    // Update NPC's schedule (location and activity)
    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void UpdateSchedule(FName NewLocation, FName NewActivity);

    // Update NPC's mood and adjust animations/behavior
    UFUNCTION(BlueprintCallable, Category = "NPC")
    virtual void UpdateMood(ENPCMood NewMood);
    
    // Get NPC's unique ID
    UFUNCTION(BlueprintPure, Category = "NPC")
    FName GetNPCId() const { return NPCId; }
    
    // Get NPC's display name
    UFUNCTION(BlueprintPure, Category = "NPC")
    FText GetDisplayName() const { return DisplayName; }
    
    // Get NPC's current mood
    UFUNCTION(BlueprintPure, Category = "NPC")
    ENPCMood GetCurrentMood() const { return CurrentMood; }
    
    // Get NPC's current location ID
    UFUNCTION(BlueprintPure, Category = "NPC")
    FName GetCurrentLocationId() const { return CurrentLocationId; }
    
    // Get NPC's current activity
    UFUNCTION(BlueprintPure, Category = "NPC")
    FName GetCurrentActivity() const { return CurrentActivity; }

    // Check if player has interacted with this NPC today
    UFUNCTION(BlueprintPure, Category = "NPC")
    bool HasPlayerInteractedToday() const;

protected:
    // Initialize NPC with the game systems
    virtual void InitializeNPC();

    // Get game mode reference
    UTimeLoopGameMode* GetTimeLoopGameMode() const;
    
    // Get NPC scheduler reference
    UNPCScheduler* GetNPCScheduler() const;
    
    // Get dialogue manager reference
    UDialogueManager* GetDialogueManager() const;

protected:
    // The unique identifier for this NPC
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
    FName NPCId;

    // The display name of this NPC
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
    FText DisplayName;
    
    // The current mood of the NPC
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    ENPCMood CurrentMood;
    
    // The current location ID of the NPC
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    FName CurrentLocationId;
    
    // The current activity of the NPC
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    FName CurrentActivity;
    
    // The ID of this NPC's dialogue tree
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC")
    FName DialogueTreeId;

    // Whether the NPC has been registered with the scheduler
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    bool bRegisteredWithScheduler;
};
