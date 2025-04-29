# Time Loop Game - Unreal Engine Configuration

## Project Settings

### General
- **Project Name:** TimeLoop
- **Engine Version:** Unreal Engine 5.3
- **Target Platforms:** Windows, macOS (primary), Linux (secondary)
- **Folder Structure:** Standard UE5 with Custom Systems Folder

### Programming
- **Language:** C++
- **Code Style:** Unreal Standard
- **License Header:** GNU GPL v3.0 (see LICENSE_HEADER.txt)

### Graphics
- **Rendering Pipeline:** Lumen for Global Illumination
- **Base Style:** Stylized Realism
- **Target Hardware:** Mid-range GPU (GTX 1660 or equivalent)
- **Target Resolution:** 1080p-1440p

## Build Configuration

### Modules
- **TimeLoop (Primary Game Module)**
- **TimeLoopCore (Core Systems)**
- **TimeLoopEditor (Editor Extensions)**

### Compiler Settings
```
OptimizeCode=CodeOptimization.InNonDebug
DebugSymbols=true
DebugGame=true
```

## Core Systems

### Time System
- **Time Manager:** Handles game time progression and day/night cycle
- **Time Loop Manager:** Manages loop resets and persistent data

### Character System
- **NPC Framework:** Modular NPC system with schedules and relationships
- **Player System:** First-person/third-person switchable perspective

### Environment System
- **Weather Controller:** Dynamic weather transitions
- **Day/Night Cycle:** Realistic lighting changes
- **Skybox Manager:** Handles sky appearance based on time and weather

### World System
- **Location Manager:** Controls available locations and transitions
- **World State Tracker:** Tracks changes to the world across loops

## Input Mapping

### Keyboard & Mouse
- **Movement:** WASD
- **Camera:** Mouse Look
- **Interaction:** E
- **Sprint:** Shift
- **Journal:** J
- **Inventory:** I
- **Menu:** Escape

### Controller
- **Movement:** Left Stick
- **Camera:** Right Stick  
- **Interaction:** X/A
- **Sprint:** R2/RT
- **Journal:** D-Pad Up
- **Inventory:** D-Pad Right
- **Menu:** Start

## Testing Framework

### Automated Tests
- **Time System Tests:** Verify time progression and loop resets
- **NPC Schedule Tests:** Validate daily routines and interactions
- **Save System Tests:** Ensure persistence works correctly

### Debug Tools
- **Time Control Panel:** Adjust time freely
- **Weather Control:** Force weather states
- **NPC Debug Mode:** Visualize schedules and paths
