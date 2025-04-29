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
#include "GameFramework/Actor.h"
#include "TestLevelManager.generated.h"

class ASkyboxManager;
class UTimeManager;
class ANPCCharacter;

/**
 * ATestLevelManager - Manages the test level environment for development
 * This actor sets up a basic testing environment with control widgets
 * for adjusting time of day, weather, and other testing parameters.
 */
UCLASS()
class TIMELOOP_API ATestLevelManager : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ATestLevelManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Set time of day for testing
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void SetTestTimeOfDay(int32 Hour, int32 Minute);
    
    // Set weather condition for testing
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void SetTestWeather(const FString& WeatherType);
    
    // Toggle accelerated time for testing
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void ToggleAcceleratedTime(bool bEnabled, float TimeScale = 60.0f);
    
    // Spawn test NPCs in the level
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void SpawnTestNPCs(int32 Count = 5);
    
    // Reset the test level to initial state
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void ResetTestLevel();
      // Toggle debugging visualizations
    UFUNCTION(BlueprintCallable, Category = "Testing")
    void ToggleDebuggingVisuals(bool bEnabled);
    
    // Get the SkyboxManager
    UFUNCTION(BlueprintPure, Category = "Testing")
    ASkyboxManager* GetSkyboxManager() const { return SkyboxManager; }
    
    // Get the TimeManager from the game mode
    UFUNCTION(BlueprintPure, Category = "Testing")
    UTimeManager* GetTimeManager() const;

protected:
    // Initialize test components
    void InitializeTestComponents();
    
    // Create debug UI elements
    void CreateDebugUI();
    
    // Reference to the skybox manager
    UPROPERTY()
    ASkyboxManager* SkyboxManager;
    
    // Debug widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
    TSubclassOf<UUserWidget> DebugWidgetClass;
    
    // Debug widget instance
    UPROPERTY()
    UUserWidget* DebugWidget;
    
    // Test NPC spawn locations
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Testing")
    TArray<FVector> TestNPCLocations;
    
    // Test NPC class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Testing")
    TSubclassOf<ANPCCharacter> TestNPCClass;
    
    // Whether debugging visualizations are enabled
    UPROPERTY(BlueprintReadWrite, Category = "Testing")
    bool bDebuggingVisualsEnabled;
    
    // Whether accelerated time is enabled
    UPROPERTY(BlueprintReadWrite, Category = "Testing")
    bool bAcceleratedTimeEnabled;
    
    // Original time scale before acceleration
    UPROPERTY(BlueprintReadWrite, Category = "Testing")
    float OriginalTimeScale;
    
    // Get reference to the time manager
    UTimeManager* GetTimeManager() const;

public:
    // Skybox manager reference
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing")
    ASkyboxManager* SkyboxManager;
    
    // Debug widget class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing|UI")
    TSubclassOf<class UUserWidget> DebugWidgetClass;
    
    // Debug widget instance
    UPROPERTY(BlueprintReadOnly, Category = "Testing|UI")
    class UUserWidget* DebugWidget;
    
    // Test NPC class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing|NPCs")
    TSubclassOf<class ANPCCharacter> TestNPCClass;
    
    // Array of spawned test NPCs
    UPROPERTY(BlueprintReadOnly, Category = "Testing|NPCs")
    TArray<class ANPCCharacter*> TestNPCs;
    
    // Test locations for NPCs
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing|NPCs")
    TArray<FVector> TestNPCLocations;
    
    // Whether debugging visuals are enabled
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing")
    bool bDebuggingVisualsEnabled;
    
    // Whether accelerated time is enabled
    UPROPERTY(BlueprintReadOnly, Category = "Testing")
    bool bAcceleratedTimeEnabled;
    
    // Original time scale before acceleration
    UPROPERTY(BlueprintReadOnly, Category = "Testing")
    float OriginalTimeScale;
};
