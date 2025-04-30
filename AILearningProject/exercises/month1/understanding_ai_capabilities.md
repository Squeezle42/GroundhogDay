# Month 1: Foundation - AI Capabilities Exercise

## Exercise: Understanding AI Response Patterns

This exercise will help you understand how different prompting styles affect AI responses, particularly for game development tasks.

### Objective
Learn how phrasing, specificity, and context affect the quality of AI-generated code for your game project.

### Setup
1. Create a new file in your game project or use the sandbox environment of your choice
2. Prepare a list of game development tasks of varying complexity (provided below)
3. Have the templates from the AILearningProject ready for reference

### Tasks

#### Task 1: Compare Vague vs. Specific Prompts
For each of the following tasks, try both versions of the prompt and compare the results:

**Feature: Player Movement System**

*Vague Prompt:*
```
Create a player movement system for my game.
```

*Specific Prompt:*
```
Create a player movement system for a browser-based adventure game using keyboard controls (WASD or arrow keys). 
The movement should be smooth with acceleration/deceleration, avoid direct DOM manipulation for performance, 
and integrate with an existing game loop that runs at 60FPS. The player's position should be stored in a game state object.
```

**Feature: Day/Night Cycle**

*Vague Prompt:*
```
Add a day/night cycle to my game.
```

*Specific Prompt:*
```
Create a day/night cycle for my browser game that:
- Completes a full cycle every 10 minutes of real-time
- Smoothly transitions between 4 states (morning, day, evening, night)
- Affects the game's visual appearance through CSS filter adjustments
- Updates a UI element showing the current time
- Triggers specific events at different times of day
```

#### Task 2: Experiment with Context Importance
Try these variations and note the differences:

**Variation 1: No Context**
```
Create a function to save game progress to local storage.
```

**Variation 2: With Technical Context**
```
I'm building a JavaScript game that needs to save progress to localStorage. 
The game state includes player position, inventory items, completed quests, and NPC relationships.
Create a function to save this game state and another to load it when the game starts.
```

**Variation 3: With Technical + Game Design Context**
```
I'm building a "Groundhog Day" style game where the player repeats the same day over and over.
The game needs to track what changes persist between loops (like player knowledge and special items) 
and what resets (like NPC positions and time of day).

Create a save system using localStorage that handles this unique requirement. The system should:
1. Save "persistent" player state between day loops (knowledge, special items)
2. Reset "temporary" state when starting a new day
3. Include functions to save, load, and reset appropriate parts of the game state
```

#### Task 3: Analyze AI's Understanding of Code
Provide a snippet of your game's code to the AI and ask it to explain what it does. Then, assess the accuracy and depth of its understanding.

**Example Code to Share:**
```javascript
function toggleViewMode() {
    const imageView = document.getElementById('image-view');
    const scene3dContainer = document.getElementById('scene3d-container');
    const toggleButton = document.getElementById('toggle-view-mode');
    
    if (!imageView || !scene3dContainer) return;
    
    // Toggle visibility
    if (imageView.style.display === 'none') {
        // Switch to 2D mode
        imageView.style.display = 'flex';
        scene3dContainer.style.display = 'none';
        if (toggleButton) toggleButton.textContent = 'Switch to 3D View';
        
        // Show notification
        showNotification('Switched to 2D view mode.');
    } else {
        // Switch to 3D mode
        imageView.style.display = 'none';
        scene3dContainer.style.display = 'block';
        if (toggleButton) toggleButton.textContent = 'Switch to 2D View';
        
        // Initialize 3D scene if needed
        initializeScene3D();
        
        // Show notification
        showNotification('Switched to 3D view mode. This feature is still in development.');
    }
}
```

**Prompt for Analysis:**
```
Here's a function from my game that toggles between 2D and 3D view modes. 
Please explain what this function does, any potential issues you see, 
and how you would improve it.
```

### Recording Your Results

For each task, document the following:
1. Both prompts used
2. Key differences in the AI responses
3. Which aspects of the more detailed prompt had the biggest impact
4. How you would improve your prompts for future similar requests

### Reflection Questions

After completing the exercises, consider:
1. What patterns did you notice in how the AI responds to different types of prompts?
2. How important was domain-specific terminology for game development?
3. Did providing context about your specific game (Groundhog Day time loop) improve the relevance of responses?
4. What will you change about your prompting style based on what you've learned?

### Application to Your Game

Choose one feature from your Groundhog Day game that you want to implement next. Create three increasingly detailed prompts for this feature, applying what you've learned. Test each one and document the results.

### Advanced Challenge

Try creating a "progressive prompt" where you start with a high-level request and then iteratively refine the solution through a series of follow-up prompts, each adding more detail or addressing issues in the previous response.

---

*This exercise is aligned with Month 1, Week 1 of your AI-Assisted Development Course as outlined in LearningAI.md*
