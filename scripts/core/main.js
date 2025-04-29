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

/**
 * Main application entry point
 */
document.addEventListener('DOMContentLoaded', () => {
    console.log('Time Loop game initializing...');
    
    // Hide loading screen after a delay
    setTimeout(() => {
        const loadingScreen = document.getElementById('loading-screen');
        if (loadingScreen) {
            loadingScreen.classList.add('hidden');
        }
    }, 2000);
    
    // Initialize 3D scene functionality
    // This is handled by scene3d-manager.js
    
    // Initialize view toggling
    // This is handled by view-manager.js
    
    // Show welcome message after loading
    setTimeout(() => {
        // Create notification container
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        // Create welcome notification
        const notification = document.createElement('div');
        notification.className = 'notification quest';
        notification.textContent = 'Welcome to Time Loop! Try exploring in 3D mode using the toggle button.';
        container.appendChild(notification);
        
        // Fade out after 8 seconds
        setTimeout(() => {
            notification.classList.add('fade-out');
        }, 8000);
    }, 3000);
    
    // Listen for toggle view button click
    const toggleButton = document.getElementById('toggle-view-mode');
    if (toggleButton) {
        toggleButton.addEventListener('click', () => {
            console.log('Toggling view mode...');
        });
    }
});

/**
 * End of main.js
 */
