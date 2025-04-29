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
#include "GameFramework/Character.h"
#include "TimeLoopPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInteractionComponent;
class UInventoryComponent;

/**
 * ATimeLoopPlayerCharacter - The player character for the Time Loop game
 */
UCLASS()
class TIMELOOP_API ATimeLoopPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATimeLoopPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Move forward/backward input handler
    void MoveForward(float Value);
    
    // Move right/left input handler
    void MoveRight(float Value);
    
    // Look up/down input handler
    void LookUp(float Value);
    
    // Look right/left input handler
    void Turn(float Value);
    
    // Sprint input handler (hold)
    void StartSprinting();
    
    // Sprint input handler (release)
    void StopSprinting();
    
    // Interact input handler
    void Interact();
    
    // Journal toggle input handler
    void ToggleJournal();
    
    // Inventory toggle input handler
    void ToggleInventory();
    
    // Check if the player is interacting with something
    UFUNCTION(BlueprintPure, Category = "Interaction")
    bool IsInteracting() const { return bIsInteracting; }
    
    // Get the player's current energy level (0-100)
    UFUNCTION(BlueprintPure, Category = "Status")
    float GetEnergyLevel() const { return EnergyLevel; }
    
    // Modify the player's energy level
    UFUNCTION(BlueprintCallable, Category = "Status")
    void ModifyEnergyLevel(float Delta);

protected:
    // Update interaction detection
    void UpdateInteractionDetection();

protected:
    // Camera boom positions the camera behind the character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;
    
    // Follow camera to view the character
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;
    
    // Interaction component for detecting and interacting with objects/NPCs
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    UInteractionComponent* InteractionComponent;
    
    // Inventory component for storing items
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    UInventoryComponent* InventoryComponent;
    
    // Maximum interaction distance
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
    float MaxInteractionDistance;
    
    // Energy level (stamina) of the player (0-100)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float EnergyLevel;
    
    // Energy drain rate when sprinting (energy per second)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0.0"))
    float SprintEnergyDrainRate;
    
    // Energy regeneration rate when not sprinting (energy per second)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (ClampMin = "0.0"))
    float EnergyRegenerationRate;
    
    // Base walking speed
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    float BaseWalkSpeed;
    
    // Sprinting speed
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    float SprintSpeed;
    
    // Whether the player is currently sprinting
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool bIsSprinting;
    
    // Whether the player is currently interacting with something
    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    bool bIsInteracting;
};
