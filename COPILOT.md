# Time Loop: Development Guide

## Current Status (April 2025)
We are at the beginning of Phase 1 (Foundation) as outlined in our roadmap. The focus is on setting up the core Unreal Engine systems for our time loop game.

## Project Overview
"Time Loop" is a narrative-driven adventure game where the player character, Morgan Chase, is trapped in a time loop in the small town of Pinewood Hollow. The game focuses on exploration, relationship building, puzzle-solving, and personal growth as Morgan attempts to break free from the loop.

## Current Development Focus: Unreal Engine Core Systems
We are currently focused on implementing the foundation of our game in Unreal Engine 5.2 with the following core systems:

### 1. Time System
- Time management (game clock, day progression)
- Day/night cycle framework
- Time loop reset mechanism
- Persistent knowledge tracking

### 2. Character System
- Player character controller
- NPC base class
- NPC scheduling framework
- Character interaction system

### 3. World System
- Town location management
- Weather system
- Interactive objects framework
- Environment state tracking

### 4. Dialogue System
- Dialogue manager
- Conversation tree structure
- Knowledge-based dialogue options
- Relationship tracking

### 5. Quest System
- Quest manager
- Quest state tracking
- Time loop-aware quest progression
- Event trigger system

### 6. Save/Load System
- Game state serialization
- Cross-loop persistence
- Player knowledge retention

## Implementation Steps

### Phase 1.1: Project Setup (Current Focus)
1. ✅ Define project structure and architecture
2. ✅ Create documentation for systems and requirements
3. ⏩ Initialize Unreal Engine project with correct settings
4. ⏩ Set up version control (Git + LFS)
5. ⏩ Implement basic directory structure in Unreal

### Phase 1.2: Core Framework Implementation
1. ⏩ Create base C++ classes for core systems
2. ⏩ Implement TimeManager class
3. ⏩ Set up player character controller
4. ⏩ Create world/level framework for town
5. ⏩ Implement basic time loop reset

### Phase 1.3: Basic Gameplay Prototype
1. ⏩ Create placeholder town square environment
2. ⏩ Implement player movement and interaction
3. ⏩ Add simple NPC with basic daily schedule
4. ⏩ Create time UI elements (clock, day counter)
5. ⏩ Test basic loop reset functionality

## Development Standards

### Code Structure
- Use Unreal naming conventions (e.g., UCLASS(), UPROPERTY())
- Organize systems in their respective folders under Source/TimeLoop
- Keep core game logic in C++, with Blueprint exposure for content creation
- Document all classes and major functions with comments

### Testing Approach
- Test each system individually as it's implemented
- Create a test map for each major feature
- Maintain a test checklist for core functionality
- Implement automated tests for critical systems

## Next Immediate Tasks

1. **Initialize Unreal Project**
   - Create new C++ project with First Person template
   - Configure project settings per UNREAL_SETUP.md
   - Set up Git repository with LFS for asset tracking

2. **Implement TimeManager Class**
   - Create basic time tracking functionality
   - Implement game clock with hour/minute progression
   - Add time acceleration control
   - Create day completion detection

3. **Basic Player Setup**
   - Modify the template first person character for our needs
   - Add interaction system (raycasting for interactive objects)
   - Implement player state that persists across loop resets

4. **Simple Test Environment**
   - Create basic town square layout
   - Add simple interactive objects (doors, items)
   - Implement day/night lighting changes

5. **Initial Time Loop Reset**
   - Create loop reset trigger (time-based or manual)
   - Implement state reset functionality
   - Add knowledge persistence framework

## Resources
- [Unreal Engine 5.2 Documentation](https://docs.unrealengine.com/5.2/en-US/)
- Project documentation in /docs folder
- PHASE1_PLAN.md for detailed implementation steps
- GROUNDHOGDAY.md for game design details

## Collaboration Notes
- Create a branch for each major system implementation
- Use pull requests with code reviews before merging to main
- Keep regular backups of the project
- Document any deviations from the original plan

This guide will be updated regularly as development progresses. The next update will focus on details of implementing the first playable prototype.
