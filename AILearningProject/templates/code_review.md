# Code Review Template

Use this template when asking AI to review code for your game.

## Code Context
```
This code is part of [COMPONENT/FEATURE] in my [GAME TYPE].
It was written to [PURPOSE/GOAL].
```

## Code to Review
```javascript
// Paste your code here
function example() {
    // Your code
}
```

## Review Focus
```
Please focus your review on:
- [ASPECT 1] (e.g., performance, readability, security)
- [ASPECT 2] (e.g., best practices, optimization)
- [ASPECT 3] (e.g., edge cases, error handling)
```

## Specific Questions
```
I'm particularly interested in:
1. [QUESTION 1]
2. [QUESTION 2]
3. [QUESTION 3]
```

## Desired Feedback Format
```
Please provide:
1. Overall assessment
2. Specific recommendations for improvement
3. Code examples for suggested changes
4. Any patterns or anti-patterns you notice
```

---

# Example Usage

## Code Context
```
This code is part of the notification system in my browser-based adventure game.
It was written to display toast messages to the player for game events, but I want to improve it to support persistent notifications.
```

## Code to Review
```javascript
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

## Review Focus
```
Please focus your review on:
- How to modify this to support persistent notifications
- Best practices for DOM manipulation
- Performance considerations with multiple notifications
```

## Specific Questions
```
I'm particularly interested in:
1. How can I add close (X) buttons to each notification?
2. What's the best way to implement a "don't show again" feature?
3. How can I ensure notifications stack properly without overlapping?
```

## Desired Feedback Format
```
Please provide:
1. Overall assessment of the current implementation
2. Specific recommendations for adding close buttons and persistence
3. Code examples showing how to implement these features
4. Any potential performance issues with the current approach
```
