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

#include "NPCCharacter.h"
#include "TimeLoop/TimeLoopGameMode.h"
#include "Systems/CharacterSystem/NPCScheduler.h"
#include "Systems/DialogueSystem/DialogueManager.h"
#include "Kismet/GameplayStatics.h"

ANPCCharacter::ANPCCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Default initialization
    NPCId = NAME_None;
    CurrentMood = ENPCMood::Neutral;
    CurrentLocationId = NAME_None;
    CurrentActivity = NAME_None;
    DialogueTreeId = NAME_None;
    bRegisteredWithScheduler = false;
}

void ANPCCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Make sure NPCId is set
    if (NPCId == NAME_None)
    {
        UE_LOG(LogTemp, Error, TEXT("NPCCharacter: NPCId not set for %s"), *GetName());
    }
    else
    {
        // Initialize NPC
        InitializeNPC();
    }
}

void ANPCCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ANPCCharacter::Interact()
{
    // Get NPC scheduler
    UNPCScheduler* Scheduler = GetNPCScheduler();
    if (Scheduler)
    {
        // Mark that player has interacted with this NPC today
        Scheduler->SetNPCInteracted(NPCId);
    }
    
    // By default, start dialogue when interacting
    StartDialogue();
}

bool ANPCCharacter::StartDialogue()
{
    // Make sure we have a dialogue tree set
    if (DialogueTreeId == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: Cannot start dialogue, no dialogue tree set for %s"), *NPCId.ToString());
        return false;
    }
    
    // Get dialogue manager
    UDialogueManager* DialogueManager = GetDialogueManager();
    if (!DialogueManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: Cannot start dialogue, no dialogue manager found"));
        return false;
    }
    
    // Start dialogue
    bool Result = DialogueManager->StartDialogue(DialogueTreeId);
    
    if (Result)
    {
        UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: Started dialogue with %s"), *NPCId.ToString());
    }
    
    return Result;
}

void ANPCCharacter::UpdateSchedule(FName NewLocation, FName NewActivity)
{
    // Update location and activity
    CurrentLocationId = NewLocation;
    CurrentActivity = NewActivity;
    
    // Log schedule update
    UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: %s updated schedule - Location: %s, Activity: %s"), 
        *NPCId.ToString(), *NewLocation.ToString(), *NewActivity.ToString());
    
    // TODO: Add logic to move NPC to new location and start appropriate animation
    // This will be implemented in a future update
}

void ANPCCharacter::UpdateMood(ENPCMood NewMood)
{
    // Update mood
    CurrentMood = NewMood;
    
    UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: %s mood changed to %d"), 
        *NPCId.ToString(), static_cast<int32>(NewMood));
    
    // TODO: Update animations and behavior based on mood
    // This will be implemented in a future update
}

bool ANPCCharacter::HasPlayerInteractedToday() const
{
    // Get NPC scheduler
    UNPCScheduler* Scheduler = GetNPCScheduler();
    if (Scheduler)
    {
        return Scheduler->HasInteractedToday(NPCId);
    }
    return false;
}

void ANPCCharacter::InitializeNPC()
{
    // Get NPC scheduler
    UNPCScheduler* Scheduler = GetNPCScheduler();
    if (Scheduler && !bRegisteredWithScheduler)
    {
        // Register with scheduler
        Scheduler->RegisterNPC(NPCId, this);
        bRegisteredWithScheduler = true;
        
        UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: %s registered with scheduler"), *NPCId.ToString());
    }
}

UTimeLoopGameMode* ANPCCharacter::GetTimeLoopGameMode() const
{
    return Cast<UTimeLoopGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

UNPCScheduler* ANPCCharacter::GetNPCScheduler() const
{
    UTimeLoopGameMode* GameMode = GetTimeLoopGameMode();
    if (GameMode)
    {
        return GameMode->GetNPCScheduler();
    }
    return nullptr;
}

UDialogueManager* ANPCCharacter::GetDialogueManager() const
{
    UTimeLoopGameMode* GameMode = GetTimeLoopGameMode();
    if (GameMode)
    {
        return GameMode->GetDialogueManager();
    }
    return nullptr;
}
