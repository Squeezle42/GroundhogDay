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
    
    // Implement progressive loading
    const loadingBar = document.getElementById('loading-bar');
    const loadingText = document.getElementById('loading-text');
    let progress = 42; // Starting at 42% as shown in the HTML
    
    // Simulate loading progress
    const loadingInterval = setInterval(() => {
        progress += Math.random() * 10;
        if (progress >= 100) {
            progress = 100;
            clearInterval(loadingInterval);
            
            // Hide loading screen after completing to 100%
            setTimeout(() => {
                const loadingScreen = document.getElementById('loading-screen');
                if (loadingScreen) {
                    loadingScreen.classList.add('hidden');
                    
                    // Initialize game after loading completes
                    initializeGame();
                }
            }, 500);
        }
        
        // Update loading bar and text
        if (loadingBar) loadingBar.style.width = `${progress}%`;
        if (loadingText) loadingText.textContent = `Loading ${Math.floor(progress)}%`;
    }, 200);
    
    // Initialize 3D scene functionality
    // This is handled by scene3d-manager.js
      // Initialize view toggling is handled by view-manager.js
    
    /**
     * Initialize the game after loading completes
     */
    function initializeGame() {
        console.log('Game loaded - initializing gameplay systems...');
        
        // Initialize game systems
        // 1. Show welcome notification
        showWelcomeNotification();
        
        // 2. Initialize UI elements
        initializeUI();
        
        // 3. Initialize click handlers for buttons
        setupEventListeners();
    }
    
    /**
     * Display welcome notification
     */
    function showWelcomeNotification() {
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
            
            // Remove from DOM after fade animation completes
            setTimeout(() => {
                notification.remove();
            }, 1000);
        }, 8000);
    }
      /**
     * Initialize UI elements
     */
    function initializeUI() {
        // Show the main game container
        const gameContainer = document.getElementById('game-container');
        if (gameContainer) {
            gameContainer.style.visibility = 'visible';
            gameContainer.style.opacity = '1';
        }
        
        // Update the time display
        updateTimeDisplay('6:00 AM', 1);
        
        // Initialize any other UI components
        console.log('UI initialized');
    }
    
    /**
     * Set up event listeners for interactive elements
     */
    function setupEventListeners() {
        // Toggle view button (2D/3D)
        const toggleButton = document.getElementById('toggle-view-mode');
        if (toggleButton) {
            toggleButton.addEventListener('click', () => {
                console.log('Toggling view mode...');
                toggleViewMode();
            });
        }
        
        // Journal button
        const journalButton = document.getElementById('journal-button');
        if (journalButton) {
            journalButton.addEventListener('click', () => {
                console.log('Opening journal...');
                toggleJournal();
            });
        }
        
        // Inventory button
        const inventoryButton = document.getElementById('inventory-button');
        if (inventoryButton) {
            inventoryButton.addEventListener('click', () => {
                console.log('Opening inventory...');
                toggleInventory();
            });
        }
        
        // Menu button
        const menuButton = document.getElementById('menu-button');
        if (menuButton) {
            menuButton.addEventListener('click', () => {
                console.log('Opening menu...');
                toggleMenu();
            });
        }
        
        // Guide button (42)
        const guideButton = document.getElementById('guide-button');
        if (guideButton) {
            guideButton.addEventListener('click', () => {
                console.log('Guide activated!');
                showGuideMessage();
            });
        }
        
        console.log('Event listeners initialized');
    }
    
    /**
     * Toggle between 2D and 3D view modes
     */
    function toggleViewMode() {
        const imageView = document.getElementById('image-view');
        const scene3dContainer = document.getElementById('scene3d-container');
        
        if (imageView && scene3dContainer) {
            if (imageView.style.display !== 'none') {
                // Switch to 3D view
                imageView.style.display = 'none';
                scene3dContainer.style.display = 'block';
            } else {
                // Switch to 2D view
                imageView.style.display = 'block';
                scene3dContainer.style.display = 'none';
            }
        }
    }
    
    /**
     * Update the time display in the UI
     */
    function updateTimeDisplay(timeString, day) {
        const timeDisplay = document.getElementById('time-display');
        const dayCounter = document.getElementById('day-counter');
        
        if (timeDisplay) {
            timeDisplay.textContent = timeString;
        }
        
        if (dayCounter) {
            dayCounter.textContent = `Day ${day}`;
        }
    }
    
    /**
     * Display a helpful guide message
     */
    function showGuideMessage() {
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        const notification = document.createElement('div');
        notification.className = 'notification guide';
        notification.textContent = "DON'T PANIC! You're caught in a time loop. Each day repeats, but your knowledge carries over.";
        container.appendChild(notification);
        
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 10000);
    }
    
    // Placeholder functions for UI toggles
    function toggleJournal() {
        const journalContainer = document.getElementById('journal-container');
        if (journalContainer) {
            journalContainer.classList.toggle('hidden');
        }
    }
    
    function toggleInventory() {
        // To be implemented
        console.log('Inventory toggle not yet implemented');
    }
    
    function toggleMenu() {
        const mainMenu = document.getElementById('main-menu');
        if (mainMenu) {
            mainMenu.classList.toggle('hidden');
        }
    }
});

// Add styles to ensure game container is initially hidden until loading completes
document.addEventListener('DOMContentLoaded', function() {
    const style = document.createElement('style');
    style.textContent = `
        #game-container {
            visibility: hidden;
            opacity: 0;
            transition: opacity 1s ease-in-out;
        }
    `;
    document.head.appendChild(style);
});

/**
 * End of main.js
 */
