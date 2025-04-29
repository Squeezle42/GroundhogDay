# Time Loop Game - Project Bible

## Code Practices & Architecture

### Unreal Engine Architecture
*   **Game Mode Subsystems:** Core game functionality divided into subsystem managers for time, NPCs, quests, and dialogue
*   **Component-Based Architecture:** Using Unreal's component system for extensible game objects
*   **State Management:** Centralized game state with persistence across time loops
*   **Event-Driven Design:** Using delegates and event dispatchers for communication between systems

### Directory Structure
```
Source/
  TimeLoop/
    TimeLoopGameMode.cpp/.h        # Core game mode
    Systems/
      TimeSystem/                  # Time and loop management
      CharacterSystem/             # NPC and character management
      QuestSystem/                 # Quest and knowledge tracking
      DialogueSystem/              # Dialogue and conversation system
      LocationSystem/              # Environment and location management
      UISystem/                    # User interface elements
    Player/                        # Player character and controllers
    Objects/                       # Interactive game objects
    Data/                          # Data assets and structures
```

### Testing Strategy
*   **Unit Tests:** For individual systems and components
*   **Integration Tests:** For interactions between subsystems
*   **Gameplay Tests:** For critical player scenarios and time loop mechanics
*   **Test Automation:** Automated testing of time progression and NPC schedules
*   **Performance Testing:** Verify performance with many NPCs and complex schedules

### Best Practices
*   **Type Safety:** Use Unreal's strong typing system and avoid casts where possible
*   **Code Documentation:** Document all systems, classes, and public methods
*   **Coding Standards:** Follow Unreal Engine coding standards for consistency
*   **Error Handling:** Robust error handling with user-friendly fallbacks
*   **Performance Optimization:** Profile and optimize NPC scheduling and time loop transitions
*   **Memory Management:** Careful management of persistent vs. ephemeral state
*   **Version Control:** Proper use of Git with meaningful commits and branches
*   **Build Pipeline:** Automated build and packaging workflows

## Implementation Timeline

| Phase | Features | Estimated Time | Description | Status |
|-------|----------|----------------|-------------|--------|
| **1. Foundation** | Core Systems | 6-8 weeks | Game framework, time system, base NPC system | ✅ Completed April 28 2025 |
| **2. Core Mechanics** | Time Loop & NPCs | 6-8 weeks | Time loop mechanics, NPC scheduling, basic interactions | ⏱️ Planning |
| **3. World Building** | Environment & Locations | 4-6 weeks | Town environment, key locations, visual transitions | ⏱️ Planning |
| **4. Character Systems** | Player & NPCs | 6-8 weeks | Player character, NPC behaviors, relationships | ⏱️ Planning |
| **5. Dialogue System** | Conversations | 4-6 weeks | Branching dialogues, knowledge tracking | ⏱️ Planning |
| **6. Quest System** | Quests & Puzzles | 6-8 weeks | Main story quests, side quests, puzzles | ⏱️ Planning |
| **7. UI Implementation** | Game Interface | 4-6 weeks | HUD, menus, journal, time display | ⏱️ Planning |
| **8. Visual Effects** | Weather & Day/Night | 3-4 weeks | Time of day changes, weather system | ⏱️ Planning |
| **9. Audio** | Music & SFX | 3-4 weeks | Adaptive music, environmental sounds | ⏱️ Planning |
| **10. Polish & Testing** | Balancing & QA | 4-6 weeks | Gameplay balance, bug fixes, optimization | ⏱️ Planning |

**Total Estimated Timeline:** 46-64 weeks (10-15 months) for a complete implementation. The timeline accounts for parallel development tracks and can be adjusted based on team size and available resources.

## Development Environment Setup

### Unreal Engine Configuration

*   **Engine Version:** Unreal Engine 5.2 (or latest stable)
*   **Project Template:** C++ Project (with Blueprint support)
*   **Target Platforms:** Windows, macOS, Linux
*   **Visual Studio Configuration:**
    *   Visual Studio 2022 with Game Development with C++ workload
    *   Recommended extensions:
        *   Visual Assist
        *   Unreal Engine Integration
        *   Productivity Power Tools
*   **IDE Settings:**
    ```json
    {
      "editor.formatOnSave": true,
      "editor.rulers": [120],
      "editor.tabSize": 4,
      "files.autoSave": "afterDelay",
      "files.trimTrailingWhitespace": true,
      "files.insertFinalNewline": true
    }
    ```
*   **C++ Snippets:** Create custom snippets for Unreal classes, properties, and delegates

### Helper Scripts & Tools

*   **Build Scripts:**
    ```batch
    @echo off
    REM build_game.bat - Builds the game for development
    "C:\Program Files\Epic Games\UE_5.2\Engine\Build\BatchFiles\Build.bat" TimeLoopEditor Win64 Development -Project="%~dp0\TimeLoop.uproject"
    
    REM package_game.bat - Packages the game for distribution
    "C:\Program Files\Epic Games\UE_5.2\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%~dp0\TimeLoop.uproject" -noP4 -platform=Win64 -clientconfig=Shipping -serverconfig=Shipping -cook -allmaps -build -stage -pak -archive -archivedirectory="%~dp0\Build"
    ```

*   **Utility Tools:**
    * Custom tools in the `Tools` directory:
        ```
        Tools/
        ├── GenerateDialogueData.py      # Convert dialogue sheets to game data
        ├── UpdateNPCSchedules.py        # Process NPC schedule spreadsheets
        ├── GenerateQuestData.py         # Convert quest designs to game data
        ├── ValidateGameData.py          # Validate all game data for consistency
        └── BuildStatsReport.py          # Generate build statistics report
        ```

### Source Control & Collaboration

*   **Git Configuration:**
    ```ini
    # .gitignore for Unreal Engine projects
    
    # Unreal Engine generated files
    Saved/
    Intermediate/
    DerivedDataCache/
    Build/
    
    # Binary files
    *.uasset
    *.umap
    
    # VS/VS Code files
    .vs/
    .vscode/
    *.sln
    *.suo
    *.opensdf
    *.sdf
    *.VC.db
    *.VC.opendb
    ```

*   **Git LFS Setup:**
    ```bash
    # Initialize Git LFS
    git lfs install
    
    # Track binary files
    git lfs track "*.uasset"
    git lfs track "*.umap"
    git lfs track "*.png"
    git lfs track "*.jpg"
    git lfs track "*.wav"
    git lfs track "*.mp3"
    ```

### Testing Best Practices

*   **Unreal Automation Testing:**
    * Use Unreal's automation testing framework for system-level tests
    * Create reusable test fixtures for common game scenarios
    * Automated testing of time system accuracy
    
*   **Test Framework Structure:**
    ```cpp
    // TimeSystemTests.cpp
    #include "CoreMinimal.h"
    #include "Misc/AutomationTest.h"
    #include "Systems/TimeSystem/TimeManager.h"
    
    IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTimeManagerProgressionTest, "TimeLoop.TimeSystem.ProgressionTest",
        EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
    
    bool FTimeManagerProgressionTest::RunTest(const FString& Parameters)
    {
        // Create time manager instance
        UTimeManager* TimeManager = NewObject<UTimeManager>();
        TimeManager->Initialize();
        
        // Test time progression
        const int32 StartHour = TimeManager->GetCurrentHour();
        const float MinutesPerSecond = 60.0f;
        
        // Simulate 1 hour of game time
        TimeManager->SetTimeScale(MinutesPerSecond);
        TimeManager->UpdateTime(60.0f);
        
        // Verify hour increased by 1
        const int32 EndHour = TimeManager->GetCurrentHour();
        TestEqual("Hour advanced by 1", EndHour, (StartHour + 1) % 24);
        
        return true;
    }
    ```

### Hot Reloading & Iteration

*   **Unreal Hot Reload:**
    * Configure for optimal C++ iteration:
    ```ini
    # DefaultEngine.ini hot reload settings
    [CoreRedirects]
    +ClassRedirects=(OldName="TimeLoopCharacter",NewName="NPCCharacter")
    +PropertyRedirects=(OldName="NPCCharacter.Mood",NewName="NPCCharacter.CurrentMood")
    ```

*   **Blueprint Compilation:**
    * Configure Blueprint compilation settings:
    ```ini
    # DefaultEngine.ini blueprint settings
    [/Script/Engine.BlueprintEditorSettings]
    bValidateUnloadedSoftReferences=True
    bEnableAutoCompilation=True
    bDrawMidpointArrowsInBlueprints=True
    bShowDetailedCompileResults=True
    ```

### Monitoring & Debugging

*   **Performance Profiling:**
    * Use Unreal Insights for performance monitoring and optimization
    * Create custom logging categories for tracking system behavior
    
*   **Debugging Tools:**
    * Custom debug visualizers for NPC schedules and time system
    * Debug commands for triggering time loop events and state changes
    * Console variables for adjusting game parameters during testing

## Game Design Reference

### Core Game Mechanics

#### Time Loop System
*   **Day Cycle:** Full day/night cycle with automated time progression
*   **Loop Reset:** Day resets at midnight or through specific triggers
*   **Time Scale:** Variable time progression speed for gameplay purposes
*   **Weather System:** Dynamic weather tied to time and story events
*   **Time of Day Effects:** Visual and gameplay effects based on time

#### NPC System
*   **Daily Schedules:** NPCs follow location-based schedules throughout the day
*   **Relationship System:** Player builds relationships that affect interactions
*   **Mood System:** NPC moods change based on player interactions and events
*   **Memory System:** NPCs remember interactions within a loop but reset on new loops
*   **Behavior Trees:** Advanced AI for realistic NPC behaviors

#### Dialogue & Knowledge System
*   **Branching Dialogue:** Conversation trees with multiple paths
*   **Knowledge Flags:** Tracking what the player has learned
*   **Conditional Dialogue:** Options that appear based on player knowledge
*   **Relationship Impact:** Dialogue choices affect NPC relationships
*   **Knowledge Persistence:** Critical knowledge persists across time loops

#### Quest & Puzzle System
*   **Main Story Quests:** Central storyline focused on breaking the time loop
*   **Character Quests:** Personal stories for each major NPC
*   **Environmental Puzzles:** Location-based puzzles that may span multiple loops
*   **Persistent Progress:** Some quest progress carries across loops
*   **Multi-day Solutions:** Puzzles requiring actions across multiple loops

### Technical Features

#### Player Interfaces
*   **Journal System:** Records discovered knowledge and tracks quest progress
*   **Time Display:** Visual indication of current time and day count
*   **Relationship Panel:** Shows player standing with various NPCs
*   **Map System:** Town map with discovered locations
*   **Inventory System:** Items carried across loops or reset with time

#### Performance Considerations
*   **NPC Optimization:** Level-of-detail system for NPC behaviors
*   **Time System Scalability:** Efficient time progression with many dependent systems
*   **Asset Streaming:** Dynamic loading of environment assets
*   **Memory Management:** Clear separation of persistent vs. loop-specific data
*   **Save System Optimization:** Efficient serialization of game state

## Copilot Development Assistant

### Copilot Mode Prompt

```
# TimeLoop Game Development Assistant

I'm developing a "Groundhog Day"-inspired time loop game in Unreal Engine using the following specifications. Please help me implement the required systems according to these guidelines:

## Architecture Guidelines:
- Follow Unreal Engine's component-based architecture patterns
- Implement C++ core systems with Blueprint exposure for content creation
- Use Unreal's delegate system for event-driven communication
- Structure the codebase using game subsystems for major features

## Core Systems:
- TimeManager: Handles game time progression and loop mechanics
- NPCScheduler: Manages NPC daily schedules and behaviors
- QuestManager: Tracks quests, objectives, and player knowledge
- DialogueManager: Handles branching conversations with NPCs
- LocationManager: Controls environment states and transitions

## Development Practices:
- Well-documented classes and public methods
- Robust error handling with appropriate logging
- Memory-efficient state management
- Performance considerations for NPC scheduling
- Clean separation between persistent and loop-specific data

## Current Phase:
[SPECIFY THE CURRENT PHASE FROM THE IMPLEMENTATION TIMELINE]

## Specific Task:
[DESCRIBE THE SPECIFIC FEATURE OR COMPONENT YOU NEED HELP WITH]

Please provide code that:
1. Follows the architecture patterns established in existing systems
2. Implements proper error handling and validation
3. Uses appropriate Unreal Engine patterns and best practices
4. Maintains good performance with many NPCs and complex time interactions
5. Includes helpful comments for future development

If modifying existing systems, ensure compatibility with dependent systems and preserve the established architecture.
```

Use this prompt when working with GitHub Copilot to ensure AI assistance aligns with the architectural decisions, coding standards, and implementation timeline specified in this document. Customize the "Current Phase" and "Specific Task" sections based on your immediate development needs.