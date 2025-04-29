/**
 * Time Loop: A Groundhog Day-Inspired Game
 * 
 * Copyright (C) 2025 Time Loop Game Development Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

# Unreal Engine Project Setup Guide

## Project Configuration

### Project Settings

- **Project Name**: TimeLoop
- **Engine Version**: Unreal Engine 5.2 (or latest stable)
- **Project Type**: C++ Project (with Blueprint support)
- **Template**: First Person
- **Target Platforms**: Windows, macOS, Linux
- **Graphics Settings**: Default with ray-tracing support
- **Scalability**: Configure for a range of hardware capabilities

### Basic Directory Structure

```
TimeLoop/
├── Config/                   # Configuration files
├── Content/                  # Game assets
│   ├── Characters/           # Character assets
│   │   ├── Player/           # Player character
│   │   └── NPCs/             # NPC characters
│   ├── Environments/         # World environments
│   │   ├── TownSquare/       # Town square area
│   │   ├── ClockTower/       # Clock tower
│   │   └── Buildings/        # Town buildings
│   ├── UI/                   # UI elements
│   ├── VFX/                  # Visual effects
│   ├── Audio/                # Audio files
│   └── TimeSystem/           # Time-related assets
├── Source/                   # C++ source code
│   └── TimeLoop/             # Main game module
├── Plugins/                  # Custom and third-party plugins
└── Documentation/            # Project documentation
```

## Required Plugins

- **Advanced Sessions**: For save/load functionality
- **Runtime Mesh Component**: For dynamic mesh modifications
- **Master System**: For advanced event systems (optional)

## Project Setup Steps

1. **Create New Project**
   - Launch Unreal Engine
   - Select "New Project"
   - Choose "First Person" template
   - Set C++ as the project type
   - Name it "TimeLoop"
   - Set your preferred location
   - Click "Create Project"

2. **Configure Source Control**
   - Set up Git with LFS
   - Create .gitignore and .gitattributes files
   - Initialize the repository
   - Make initial commit

3. **Setup Development Environment**
   - Configure Visual Studio/VS Code
   - Set up build scripts
   - Create development branches

4. **Implement Core Frameworks**
   - Set up game module structure
   - Create base classes for:
     - TimeSystem
     - CharacterSystem
     - QuestSystem
     - DialogueSystem

## Essential Classes and Components

This section outlines the core classes that need to be implemented for the TimeLoop project. These will form the foundation of our game systems.

### Source Files Structure
```
Source/TimeLoop/
├── TimeLoop.h                         # Main module header
├── TimeLoop.cpp                       # Main module source
├── TimeLoopGameMode.h                 # Game mode header
├── TimeLoopGameMode.cpp               # Game mode source
├── TimeLoopCharacter.h                # Player character header
├── TimeLoopCharacter.cpp              # Player character source
├── Systems/                           # Game systems directory
│   ├── TimeSystem/                    # Time loop system
│   │   ├── TimeManager.h              # Time management header
│   │   ├── TimeManager.cpp            # Time management source
│   │   ├── TimeLoopSaveGame.h         # Save game state header
│   │   └── TimeLoopSaveGame.cpp       # Save game state source
│   ├── CharacterSystem/               # Character system
│   │   ├── NPCBase.h                  # Base NPC class header
│   │   ├── NPCBase.cpp                # Base NPC class source
│   │   ├── NPCScheduler.h             # NPC scheduling header
│   │   └── NPCScheduler.cpp           # NPC scheduling source
│   ├── DialogueSystem/                # Dialogue system
│   │   ├── DialogueManager.h          # Dialogue manager header
│   │   ├── DialogueManager.cpp        # Dialogue manager source
│   │   ├── DialogueTypes.h            # Dialogue data types
│   │   └── DialogueInstance.h         # Dialogue instance header
│   └── QuestSystem/                   # Quest system
│       ├── QuestManager.h             # Quest manager header
│       ├── QuestManager.cpp           # Quest manager source
│       ├── QuestTypes.h               # Quest data types
│       └── QuestInstance.h            # Quest instance header
├── UI/                                # UI components
│   ├── TimeLoopHUD.h                  # HUD header
│   ├── TimeLoopHUD.cpp                # HUD source
│   ├── MainMenuWidget.h               # Main menu widget header
│   └── GameUIWidget.h                 # Game UI widget header
└── Utils/                             # Utility functions
    ├── TimeLoopStatics.h              # Static utility functions
    └── TimeLoopTypes.h                # Global type definitions
```

## Implementing the Core Time Loop System

The time loop system is the central mechanic of the game. It should handle:
1. Day progression
2. Loop resets
3. Persistent knowledge between loops
4. Time-based events
5. State tracking

See the additional implementation files for detailed code structure.
