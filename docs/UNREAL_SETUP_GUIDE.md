# Time Loop Game - Unreal Engine Setup Guide

This document outlines the steps to set up the Time Loop game project in Unreal Engine.

## Initial Setup

### 1. Prerequisites
- Unreal Engine 5.3 or later
- Visual Studio 2022 or later with C++ desktop development workload
- Git for version control
- 16GB+ RAM recommended

### 2. Creating the Project

1. Open Unreal Engine Launcher
2. Click "New Project"
3. Select "Games" category
4. Choose "C++" (not Blueprint) 
5. Choose "Basic Code" template
6. Set the following options:
   - Project Name: TimeLoop
   - Location: Your desired project folder
   - C++ Class: Defaults to AGameMode
   - Target Platform: Desktop
   - Quality Preset: Maximum Quality
   - Starter Content: Disabled
   - Raytracing: Enabled
7. Click "Create Project"

### 3. Setting Up Project Structure

1. Open the project in Unreal Editor
2. Create the following folder structure in the Content Browser:
   ```
   /Content
   ├── Characters
   │   ├── NPCs
   │   └── Player
   ├── Environment
   │   ├── Materials
   │   ├── Skybox
   │   └── Weather
   ├── Gameplay
   │   ├── Interaction
   │   ├── Quest
   │   └── TimeLoop
   ├── Levels
   │   ├── Main
   │   └── Testing
   ├── UI
   │   ├── Dialogue
   │   ├── HUD
   │   ├── Inventory
   │   ├── Journal
   │   └── Menus
   └── _Core
       ├── DataTables
       └── Managers
   ```

3. In your project's C++ structure, create the following class folders:
   ```
   /Source/TimeLoop
   ├── Characters
   │   ├── NPCs
   │   └── Player
   ├── Environment
   ├── Systems
   │   ├── CharacterSystem
   │   ├── DialogueSystem
   │   ├── QuestSystem
   │   └── TimeSystem
   └── Testing
   ```

## Setting Up Core Systems

### 1. Game Mode Setup

1. Copy `TimeLoopGameMode.h` and `TimeLoopGameMode.cpp` to your project's Source/TimeLoop directory
2. Compile the code
3. Open Project Settings > Maps & Modes
4. Set Default GameMode to TimeLoopGameMode

### 2. Time System Setup

1. Copy all files from the TimeSystem directory to your project
2. Compile the code
3. Create a new level or open an existing one
4. Add the TimeLoopGameMode to the level's World Settings

### 3. Test Environment Setup

1. Copy the SkyboxManager and TestLevelManager classes to your project
2. Create a new Test level in Content/Levels/Testing
3. Add a SkyboxManager actor to the level
4. Add a TestLevelManager actor to the level
5. Open the level Blueprint and connect it to the TestLevelManager for debugging controls

## Setting Up the Testing Level

### 1. Creating the Test Level

1. Create a new Level (File > New Level > Empty Level)
2. Name it "TestLevel" and save it in Content/Levels/Testing
3. Add a basic floor plane for movement testing
4. Add a SkyboxManager actor to the level
5. Add a TestLevelManager actor to the level

### 2. Configuring the SkyboxManager

1. Select the SkyboxManager actor
2. In the Details panel, locate or create a Material asset for the sky
3. Set appropriate colors for different times of day
4. Create particle systems for snow and rain effects

### 3. Setting Up the Player Character

1. Set the default Pawn class to TimeLoopPlayerCharacter
2. Configure input mappings in Project Settings > Input
3. Add a Player Start actor to the level

### 4. Creating Test NPCs

1. Create a Blueprint based on NPCCharacter
2. Set up simple schedules for testing
3. Place a few instances in the level

## Running the Test Environment

1. Play the test level in the editor
2. Use the TestLevelManager to:
   - Adjust time of day to see sky and lighting changes
   - Change weather conditions
   - Test NPC schedules at different times

## Additional Resources

- Check the included code documentation for detailed class descriptions
- Review the `COPILOT.md` file for development roadmap
- See `IMPLEMENTATION_PLAN.md` for system implementation details
