/**
 * Time Loop: A Groundhog Day-Inspired Game
 * 
 * Copyright (C) 2025 Time Loop Game Development Team
 */

/**
 * View mode toggle handler
 */
document.addEventListener('DOMContentLoaded', () => {
    // Get the toggle button
    const toggleButton = document.getElementById('toggle-view-mode');
    if (!toggleButton) return;
    
    // Get the view containers
    const imageView = document.getElementById('image-view');
    const scene3dContainer = document.getElementById('scene3d-container');
    
    // Track current view mode
    let is3DMode = false;
    
    // Add click handler
    toggleButton.addEventListener('click', () => {
        // Toggle mode
        is3DMode = !is3DMode;
        
        // Update button text
        toggleButton.textContent = is3DMode ? 'Show 2D View' : 'Show 3D View';
        
        // Show/hide appropriate view
        if (is3DMode) {
            // Show 3D view
            imageView.style.display = 'none';
            scene3dContainer.style.display = 'block';
            
            // Create notification about 3D mode
            createNotification('3D Mode enabled. Hover over objects to interact with them!', 'discovery');
        } else {
            // Show 2D view
            imageView.style.display = 'block';
            scene3dContainer.style.display = 'none';
            
            // Create notification about 2D mode
            createNotification('Returned to 2D Mode', 'info');
        }
    });
    
    /**
     * Create a notification
     * @param {string} message - Notification text
     * @param {string} type - Notification type (important, quest, discovery, info)
     */
    function createNotification(message, type = 'info') {
        // Get or create notification container
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        // Create notification
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        notification.textContent = message;
        container.appendChild(notification);
        
        // Fade out after 5 seconds
        setTimeout(() => {
            notification.classList.add('fade-out');
            
            // Remove from DOM after fade
            setTimeout(() => {
                container.removeChild(notification);
                
                // Remove container if empty
                if (container.childElementCount === 0) {
                    document.body.removeChild(container);
                }
            }, 500);
        }, 5000);
    }
});
