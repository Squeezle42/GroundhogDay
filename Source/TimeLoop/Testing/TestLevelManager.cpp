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

#include "TestLevelManager.h"
#include "TimeLoop/Environment/SkyboxManager.h"
#include "TimeLoop/Systems/TimeSystem/TimeManager.h"
#include "TimeLoop/Systems/CharacterSystem/NPCCharacter.h"
#include "TimeLoop/TimeLoopGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"

ATestLevelManager::ATestLevelManager()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Initialize default values
    bDebuggingVisualsEnabled = false;
    bAcceleratedTimeEnabled = false;
    OriginalTimeScale = 1.0f;
}

void ATestLevelManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Initialize test components
    InitializeTestComponents();
    
    // Create debug UI if class is set
    if (DebugWidgetClass)
    {
        CreateDebugUI();
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Initialized"));
}

void ATestLevelManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATestLevelManager::InitializeTestComponents()
{
    // Find Skybox Manager if not already set
    if (!SkyboxManager)
    {
        // Look for a SkyboxManager in the level
        for (TActorIterator<ASkyboxManager> It(GetWorld()); It; ++It)
        {
            SkyboxManager = *It;
            break;
        }
        
        // If still not found, spawn one
        if (!SkyboxManager)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            SkyboxManager = GetWorld()->SpawnActor<ASkyboxManager>(ASkyboxManager::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator, SpawnParams);
            
            if (SkyboxManager)
            {
                UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Created SkyboxManager"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Test Level Manager: Failed to create SkyboxManager"));
            }
        }
    }
    
    // Initialize test NPC locations if empty
    if (TestNPCLocations.Num() == 0)
    {
        // Add some default test locations
        TestNPCLocations.Add(FVector(300.0f, 0.0f, 100.0f));
        TestNPCLocations.Add(FVector(300.0f, 300.0f, 100.0f));
        TestNPCLocations.Add(FVector(0.0f, 300.0f, 100.0f));
        TestNPCLocations.Add(FVector(-300.0f, 300.0f, 100.0f));
        TestNPCLocations.Add(FVector(-300.0f, 0.0f, 100.0f));
    }
}

void ATestLevelManager::CreateDebugUI()
{
    // Create the debug widget
    if (DebugWidgetClass)
    {
        DebugWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), DebugWidgetClass);
        if (DebugWidget)
        {
            DebugWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Created debug UI"));
        }
    }
}

void ATestLevelManager::SetTestTimeOfDay(int32 Hour, int32 Minute)
{
    // Clamp values
    Hour = FMath::Clamp(Hour, 0, 23);
    Minute = FMath::Clamp(Minute, 0, 59);
    
    // Calculate time of day as float (hours.minutes)
    float TimeOfDay = (float)Hour + ((float)Minute / 60.0f);
    
    // Update SkyboxManager
    if (SkyboxManager)
    {
        SkyboxManager->SetTimeOfDay(TimeOfDay);
    }
    
    // Also update time in the game's time manager if available
    UTimeManager* TimeManager = GetTimeManager();
    if (TimeManager)
    {
        // This would need to be implemented in TimeManager to allow setting specific times
        // For now, we'll log that it would happen
        UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set game time to %02d:%02d"), Hour, Minute);
    }
}

void ATestLevelManager::SetTestWeather(const FString& WeatherType)
{
    if (!SkyboxManager)
    {
        return;
    }
    
    // Convert string to weather enum
    EWeatherCondition Weather = EWeatherCondition::Clear;
    
    if (WeatherType.Equals("Clear", ESearchCase::IgnoreCase))
    {
        Weather = EWeatherCondition::Clear;
    }
    else if (WeatherType.Equals("Cloudy", ESearchCase::IgnoreCase))
    {
        Weather = EWeatherCondition::Cloudy;
    }
    else if (WeatherType.Equals("LightSnow", ESearchCase::IgnoreCase) || WeatherType.Equals("Light Snow", ESearchCase::IgnoreCase))
    {
        Weather = EWeatherCondition::LightSnow;
    }
    else if (WeatherType.Equals("HeavySnow", ESearchCase::IgnoreCase) || WeatherType.Equals("Heavy Snow", ESearchCase::IgnoreCase))
    {
        Weather = EWeatherCondition::HeavySnow;
    }
    else if (WeatherType.Equals("Raining", ESearchCase::IgnoreCase) || WeatherType.Equals("Rain", ESearchCase::IgnoreCase))
    {
        Weather = EWeatherCondition::Raining;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Unknown weather type '%s', defaulting to Clear"), *WeatherType);
    }
    
    // Set weather
    SkyboxManager->SetWeatherCondition(Weather);
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set weather to %s"), *WeatherType);
}

void ATestLevelManager::ToggleAcceleratedTime(bool bEnabled, float TimeScale)
{
    // Only proceed if we can access the time manager
    UTimeManager* TimeManager = GetTimeManager();
    if (!TimeManager)
    {
        return;
    }
    
    if (bEnabled && !bAcceleratedTimeEnabled)
    {
        // Store original time scale
        OriginalTimeScale = TimeManager->TimeScale;
        
        // Set accelerated time scale
        TimeManager->SetTimeScale(TimeScale);
        bAcceleratedTimeEnabled = true;
        
        UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Accelerated time enabled (%.1fx)"), TimeScale);
    }
    else if (!bEnabled && bAcceleratedTimeEnabled)
    {
        // Restore original time scale
        TimeManager->SetTimeScale(OriginalTimeScale);
        bAcceleratedTimeEnabled = false;
        
        UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Accelerated time disabled"));
    }
}

void ATestLevelManager::SpawnTestNPCs(int32 Count)
{
    if (!TestNPCClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: No TestNPCClass set, cannot spawn NPCs"));
        return;
    }
    
    // Limit to reasonable number
    Count = FMath::Clamp(Count, 1, 20);
    
    // Make sure we have at least one location
    if (TestNPCLocations.Num() == 0)
    {
        TestNPCLocations.Add(FVector(0.0f, 0.0f, 100.0f));
    }
    
    // Spawn NPCs
    for (int32 i = 0; i < Count; ++i)
    {
        // Get location (cycle through available locations)
        const FVector& SpawnLocation = TestNPCLocations[i % TestNPCLocations.Num()];
        
        // Spawn parameters
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        // Spawn NPC
        ANPCCharacter* NPC = GetWorld()->SpawnActor<ANPCCharacter>(TestNPCClass, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
        
        if (NPC)
        {
            FName NPCId = FName(*FString::Printf(TEXT("TestNPC_%d"), i + 1));
            // Note: Would need additional setup here for real NPCs
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Spawned test NPC at location %s"), *SpawnLocation.ToString());
        }
    }
}

void ATestLevelManager::ResetTestLevel()
{
    // Reset time to morning
    SetTestTimeOfDay(6, 0); // 6 AM
    
    // Set weather to clear
    SetTestWeather("Clear");
    
    // Disable accelerated time if it was enabled
    if (bAcceleratedTimeEnabled)
    {
        ToggleAcceleratedTime(false);
    }
    
    // Reset any other test parameters
    ToggleDebuggingVisuals(false);
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Reset test level to initial state"));
}

void ATestLevelManager::ToggleDebuggingVisuals(bool bEnabled)
{
    bDebuggingVisualsEnabled = bEnabled;
    
    // Implementation would depend on what debugging visuals are available
    // For example, you could enable debug drawing for NPCs, path visualization, etc.
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Debugging visuals %s"), 
        bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

UTimeManager* ATestLevelManager::GetTimeManager() const
{
    // Get the game mode
    ATimeLoopGameMode* GameMode = Cast<ATimeLoopGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        // Return its time manager
        return GameMode->GetTimeManager();
    }
    
    return nullptr;
    if (DebugWidgetClass)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            DebugWidget = CreateWidget<UUserWidget>(PlayerController, DebugWidgetClass);
            if (DebugWidget)
            {
                DebugWidget->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Debug UI created"));
            }
        }
    }
}

void ATestLevelManager::SetTestTimeOfDay(int32 Hour, int32 Minute)
{
    // Get the time manager
    UTimeManager* TimeManager = GetTimeManager();
    if (TimeManager)
    {
        // Store current time scale
        const float CurrentTimeScale = TimeManager->TimeScale;
        
        // Pause time progression
        TimeManager->SetTimeScale(0.0f);
        
        // Use the private SetTime method through reflection
        // This is a workaround for testing purposes
        UFunction* SetTimeFunc = TimeManager->GetClass()->FindFunctionByName(FName("SetTime"));
        if (SetTimeFunc)
        {
            struct
            {
                int32 Hour;
                int32 Minute;
            } Params;
            
            Params.Hour = Hour;
            Params.Minute = Minute;
            
            TimeManager->ProcessEvent(SetTimeFunc, &Params);
            
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set test time to %02d:%02d"), Hour, Minute);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Test Level Manager: Could not find SetTime method in TimeManager"));
        }
        
        // Restore time scale
        TimeManager->SetTimeScale(CurrentTimeScale);
        
        // Update skybox if available
        if (SkyboxManager)
        {
            SkyboxManager->UpdateSkyForTimeOfDay(Hour);
        }
    }
}

void ATestLevelManager::SetTestWeather(const FString& WeatherType)
{
    // Update skybox weather if available
    if (SkyboxManager)
    {
        // Convert string to enum (will need to be updated once weather types are defined)
        if (WeatherType.Equals("Clear", ESearchCase::IgnoreCase))
        {
            // SkyboxManager->SetWeatherCondition(EWeatherCondition::Clear);
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set weather to Clear"));
        }
        else if (WeatherType.Equals("Cloudy", ESearchCase::IgnoreCase))
        {
            // SkyboxManager->SetWeatherCondition(EWeatherCondition::Cloudy);
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set weather to Cloudy"));
        }
        else if (WeatherType.Equals("Rain", ESearchCase::IgnoreCase))
        {
            // SkyboxManager->SetWeatherCondition(EWeatherCondition::Rain);
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set weather to Rain"));
        }
        else if (WeatherType.Equals("Snow", ESearchCase::IgnoreCase))
        {
            // SkyboxManager->SetWeatherCondition(EWeatherCondition::Snow);
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Set weather to Snow"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Unknown weather type '%s'"), *WeatherType);
        }
    }
}

void ATestLevelManager::ToggleAcceleratedTime(bool bEnabled, float TimeScale)
{
    UTimeManager* TimeManager = GetTimeManager();
    if (TimeManager)
    {
        if (bEnabled)
        {
            // Store original time scale before accelerating
            OriginalTimeScale = TimeManager->TimeScale;
            
            // Set accelerated time scale
            TimeManager->SetTimeScale(TimeScale);
            
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Accelerated time enabled (%.1fx)"), TimeScale);
        }
        else
        {
            // Restore original time scale
            TimeManager->SetTimeScale(OriginalTimeScale);
            
            UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Accelerated time disabled"));
        }
        
        bAcceleratedTimeEnabled = bEnabled;
    }
}

void ATestLevelManager::SpawnTestNPCs(int32 Count)
{
    // Make sure we have a valid NPC class
    if (!TestNPCClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Test Level Manager: No TestNPCClass set"));
        return;
    }
    
    // Clear existing test NPCs
    for (ANPCCharacter* NPC : TestNPCs)
    {
        if (NPC)
        {
            NPC->Destroy();
        }
    }
    TestNPCs.Empty();
    
    // Make sure we have enough locations
    while (TestNPCLocations.Num() < Count)
    {
        // Add random positions if we need more
        FVector RandomPos = FVector(
            FMath::RandRange(-500.0f, 500.0f),
            FMath::RandRange(-500.0f, 500.0f),
            100.0f
        );
        TestNPCLocations.Add(RandomPos);
    }
    
    // Spawn new test NPCs
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    for (int32 i = 0; i < Count; ++i)
    {
        FVector Location = TestNPCLocations[i % TestNPCLocations.Num()];
        FRotator Rotation = FRotator(0, FMath::RandRange(0.0f, 360.0f), 0);
        
        ANPCCharacter* NPC = GetWorld()->SpawnActor<ANPCCharacter>(TestNPCClass, Location, Rotation, SpawnParams);
        if (NPC)
        {
            // Give the NPC a unique ID
            // Note: This would need to be updated to match your actual implementation
            // NPC->NPCId = FName(*FString::Printf(TEXT("TestNPC_%d"), i));
            
            TestNPCs.Add(NPC);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Spawned %d test NPCs"), TestNPCs.Num());
}

void ATestLevelManager::ResetTestLevel()
{
    // Reset time to morning
    UTimeManager* TimeManager = GetTimeManager();
    if (TimeManager)
    {
        TimeManager->ResetToMorning();
    }
    
    // Reset weather
    SetTestWeather("Clear");
    
    // Remove test NPCs
    for (ANPCCharacter* NPC : TestNPCs)
    {
        if (NPC)
        {
            NPC->Destroy();
        }
    }
    TestNPCs.Empty();
    
    // Disable accelerated time
    if (bAcceleratedTimeEnabled)
    {
        ToggleAcceleratedTime(false);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Test level reset"));
}

void ATestLevelManager::ToggleDebuggingVisuals(bool bEnabled)
{
    bDebuggingVisualsEnabled = bEnabled;
    
    // Toggle visual debugging elements like NPC paths, schedule markers, etc.
    // This would need to be implemented based on your specific debugging needs
    
    UE_LOG(LogTemp, Warning, TEXT("Test Level Manager: Debugging visuals %s"), 
        bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

UTimeManager* ATestLevelManager::GetTimeManager() const
{
    // Get the game mode
    ATimeLoopGameMode* GameMode = Cast<ATimeLoopGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        return GameMode->GetTimeManager();
    }
    
    UE_LOG(LogTemp, Error, TEXT("Test Level Manager: Could not get TimeManager"));
    return nullptr;
}
