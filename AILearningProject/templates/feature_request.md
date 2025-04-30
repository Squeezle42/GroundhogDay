# Feature Request Template

Use this template when asking AI to help implement a new game feature.

## Context
```
I'm working on [GAME TYPE], which uses [TECHNOLOGIES/FRAMEWORKS]. 
The current feature I'm implementing is [FEATURE NAME].
This feature needs to integrate with [EXISTING SYSTEMS].
```

## Requirements
```
The feature should:
- [REQUIREMENT 1]
- [REQUIREMENT 2]
- [REQUIREMENT 3]
```

## Technical Constraints
```
- Must work with [BROWSER/ENVIRONMENT]
- Must follow [CODING PATTERNS/STYLE]
- Performance requirements: [SPECIFY IF RELEVANT]
```

## Examples/References
```
Similar functionality can be found in:
- [EXAMPLE 1]
- [EXAMPLE 2]
```

## Desired Output Format
```
Please provide:
1. A high-level approach explaining your solution strategy
2. The implementation code with clear comments
3. Any integration points with existing code
4. Potential edge cases or considerations
```

## Current Code References
```
Relevant existing code:
[CODE SNIPPET 1]
[CODE SNIPPET 2]
```

---

# Example Usage

## Context
```
I'm working on a Groundhog Day-inspired browser game using vanilla JavaScript, HTML and CSS. 
The current feature I'm implementing is a "notification system" that shows toast messages.
This feature needs to integrate with the existing UI manager and event system.
```

## Requirements
```
The feature should:
- Show toast messages that remain visible until dismissed
- Include a close (X) button to dismiss notifications
- Support an option to suppress specific message types in the future (via a "*" button)
- Support different message types (info, warning, error, quest)
```

## Technical Constraints
```
- Must work with modern browsers (Chrome, Firefox, Edge)
- Must follow our component-based approach
- Should be lightweight and not impact game performance
```

## Examples/References
```
Similar functionality can be found in:
- The dialogue system's popup mechanism
- Modern web applications with notification toasts
```

## Desired Output Format
```
Please provide:
1. A high-level approach explaining your solution strategy
2. The HTML/CSS/JS implementation with clear comments
3. Integration points with the existing UI manager
4. How to handle edge cases like multiple notifications
```

## Current Code References
```
Relevant existing code:
// Current notification implementation
function showNotification(message, type = 'info') {
    // Create notification container if it doesn't exist
    let container = document.getElementById('notification-container');
    if (!container) {
        container = document.createElement('div');
        container.id = 'notification-container';
        document.body.appendChild(container);
    }
    
    // Create notification element
    const notification = document.createElement('div');
    notification.className = `notification ${type}`;
    notification.textContent = message;
    
    // Add to container
    container.appendChild(notification);
    
    // Fade out after delay
    setTimeout(() => {
        notification.classList.add('fade-out');
        setTimeout(() => notification.remove(), 1000);
    }, 4000);
}
```
