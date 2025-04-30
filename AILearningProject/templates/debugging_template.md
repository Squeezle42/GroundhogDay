# Debugging Template

Use this template when asking AI to help debug issues in your game.

## Problem Description
```
I'm experiencing an issue with [COMPONENT/FEATURE].
Expected behavior: [WHAT SHOULD HAPPEN]
Actual behavior: [WHAT IS HAPPENING INSTEAD]
```

## Context
```
This is part of a [GAME TYPE] using [TECHNOLOGIES].
The affected feature was [RECENTLY IMPLEMENTED/CHANGED/etc.].
```

## Error Messages
```
[PASTE ANY ERROR MESSAGES, CONSOLE LOGS, ETC.]
```

## Code Snippets
```javascript
// Relevant code that might be causing the issue
[CODE SNIPPET 1]

// Related components/functions
[CODE SNIPPET 2]
```

## Attempted Solutions
```
I've already tried:
- [APPROACH 1]
- [APPROACH 2]
```

## Request
```
Please help me:
1. Identify the root cause of this issue
2. Suggest a fix with code examples
3. Explain why the issue occurred to prevent similar problems
```

---

# Example Usage

## Problem Description
```
I'm experiencing an issue with the game's image display system.
Expected behavior: Location images should display fully within the game view without being cut off
Actual behavior: Images are being partially cut off at the bottom, especially on smaller screens
```

## Context
```
This is part of a browser-based adventure game using vanilla JavaScript and CSS.
The affected feature was recently modified to handle different image aspect ratios.
```

## Error Messages
```
No console errors, but visually the images are cut off.
```

## Code Snippets
```css
/* Current image display CSS */
#location-image {
    width: 100%;
    height: 100%;
    object-fit: contain;
    max-height: calc(100vh - 190px);
}

#image-view {
    flex: 1;
    display: flex;
    flex-direction: column;
    position: relative;
    height: 100%;
}
```

```javascript
// Related viewport calculation code
function adjustForViewport() {
    const viewportHeight = window.innerHeight;
    const gameContainer = document.getElementById('game-container');
    const imageView = document.getElementById('image-view');
    const locationImage = document.getElementById('location-image');
    
    // Calculate available height for image after accounting for UI elements
    const hud = document.getElementById('hud');
    const locationHeader = document.getElementById('location-header');
    const interactionArea = document.getElementById('interaction-area');
    
    const hudHeight = hud ? hud.offsetHeight : 50;
    const headerHeight = locationHeader ? locationHeader.offsetHeight : 45;
    const interactionHeight = interactionArea ? interactionArea.offsetHeight : 65;
    
    // Calculate image container height
    const imageHeight = viewportHeight - (hudHeight + headerHeight + interactionHeight + 10);
    
    if (imageView && locationImage) {
        imageView.style.height = `${imageHeight}px`;
        locationImage.style.maxHeight = `${imageHeight}px`;
    }
}
```

## Attempted Solutions
```
I've already tried:
- Adjusting the max-height value manually
- Using different object-fit values
- Adding overflow: hidden to parent containers
```

## Request
```
Please help me:
1. Identify why the images are being cut off despite the viewport calculations
2. Provide a CSS/JS solution that ensures images display fully at all screen sizes
3. Explain the underlying issue with image container sizing
```
