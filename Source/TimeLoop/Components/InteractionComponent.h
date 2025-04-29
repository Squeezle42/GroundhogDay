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

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

/**
 * UInteractionComponent - Component that handles interactions with other actors
 * This component enables detecting and interacting with objects and NPCs in the game world
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TIMELOOP_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UInteractionComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Trigger an interaction with the currently focused interactable
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    bool TriggerInteraction();
    
    // Get the current interaction target
    UFUNCTION(BlueprintPure, Category = "Interaction")
    AActor* GetCurrentInteractionTarget() const { return CurrentInteractionTarget; }
    
    // Check if there is a valid interaction target
    UFUNCTION(BlueprintPure, Category = "Interaction")
    bool HasValidInteractionTarget() const { return CurrentInteractionTarget != nullptr; }
    
    // Get the maximum interaction distance
    UFUNCTION(BlueprintPure, Category = "Interaction")
    float GetMaxInteractionDistance() const { return MaxInteractionDistance; }
    
    // Set the maximum interaction distance
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetMaxInteractionDistance(float NewDistance) { MaxInteractionDistance = NewDistance; }

protected:
    // Update the current interaction target based on a raycast
    void UpdateInteractionTarget();
    
    // The current actor that can be interacted with
    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    AActor* CurrentInteractionTarget;
    
    // Maximum distance for interactions
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    float MaxInteractionDistance;
    
    // Whether debug drawing is enabled
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
    bool bDrawDebug;
};
