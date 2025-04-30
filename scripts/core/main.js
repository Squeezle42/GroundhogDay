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
        progress += Math.random() * 8;
        if (progress >= 100) {
            progress = 100;
            clearInterval(loadingInterval);
            
            // Update final loading state
            if (loadingBar) loadingBar.style.width = `${progress}%`;
            if (loadingText) loadingText.textContent = `Loading ${Math.floor(progress)}%`;
            
            // Hide loading screen after completing to 100%
            setTimeout(() => {
                const loadingScreen = document.getElementById('loading-screen');
                if (loadingScreen) {
                    loadingScreen.classList.add('hidden');
                    
                    // Calculate viewport dimensions for optimal display
                    adjustForViewport();
                    
                    // Initialize game after loading completes
                    initializeGame();
                }
            }, 800);
        }
        
        // Update loading bar and text
        if (loadingBar) loadingBar.style.width = `${progress}%`;
        if (loadingText) loadingText.textContent = `Loading ${Math.floor(progress)}%`;
    }, 200);
    
    /**
     * Adjust layout based on viewport dimensions
     */
    function adjustForViewport() {
        const viewportHeight = window.innerHeight;
        const gameContainer = document.getElementById('game-container');
        const imageView = document.getElementById('image-view');
        const locationImage = document.getElementById('location-image');
        
        // Set custom CSS variable for viewport height (used in calculations)
        document.documentElement.style.setProperty('--viewport-height', `${viewportHeight}px`);
        
        // Calculate available height for image after accounting for UI elements
        const hud = document.getElementById('hud');
        const locationHeader = document.getElementById('location-header');
        const interactionArea = document.getElementById('interaction-area');
        
        const hudHeight = hud ? hud.offsetHeight : 50;
        const headerHeight = locationHeader ? locationHeader.offsetHeight : 45;
        const interactionHeight = interactionArea ? interactionArea.offsetHeight : 65;
        
        // Calculate image container height
        const imageHeight = viewportHeight - (hudHeight + headerHeight + interactionHeight + 10); // 10px buffer
        
        if (imageView && locationImage) {
            imageView.style.height = `${imageHeight}px`;
            locationImage.style.maxHeight = `${imageHeight}px`;
            
            console.log(`Viewport adjusted: ${viewportHeight}px available, ${imageHeight}px for images`);
        }
    }
    
    // Listen for resize events to readjust layout
    window.addEventListener('resize', adjustForViewport);
      /**
     * Initialize the game after loading completes
     */
    async function initializeGame() {
        console.log('Game loaded - initializing gameplay systems...');
        
        try {
            // Start the game engine
            if (!window.timeLoopGame) {
                console.log("Game engine not found, initializing...");
                // Create game engine if it doesn't exist
                window.timeLoopGame = {
                    initialize: async function() {
                        // Load and initialize the game components
                        console.log("Setting up game engine components...");
                        
                        // Initialize location manager
                        this.locationManager = new LocationManager();
                        await this.locationManager.initialize();
                        
                        // Initialize character manager
                        this.characterManager = new CharacterManager(this);
                        await this.characterManager.initialize();
                        
                        return true;
                    },
                    
                    // Game state
                    currentDay: 1,
                    currentTime: "morning",
                    weatherConditions: "snowing"
                };
            }
            
            // Initialize the game engine
            const success = await window.timeLoopGame.initialize();
            if (!success) {
                throw new Error("Failed to initialize game engine");
            }
            
            // Initialize UI elements
            initializeUI();
            
            // Initialize click handlers for buttons
            setupEventListeners();
            
            // Show welcome notification
            showWelcomeNotification();
            
            // Start in the inn location
            if (window.timeLoopGame.locationManager) {
                window.timeLoopGame.locationManager.goToLocation('inn');
            }
            
            console.log("Game initialized successfully!");
        } catch (error) {
            console.error("Error initializing game:", error);
            
            // Show error to user
            const errorNotification = document.createElement('div');
            errorNotification.className = 'notification error';
            errorNotification.textContent = "Error loading game. Please refresh the page and try again.";
            document.body.appendChild(errorNotification);
        }
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
        
        // Add notification to container
        container.appendChild(notification);
        
        // Fade out after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 8000);
    }
      /**
     * Initialize UI elements
     */
    function initializeUI() {
        // Make game container visible
        const gameContainer = document.getElementById('game-container');
        if (gameContainer) {
            gameContainer.classList.remove('hidden');
        }
        
        // Initialize toggle view mode button
        const toggleViewButton = document.getElementById('toggle-view-mode');
        if (toggleViewButton) {
            toggleViewButton.addEventListener('click', toggleViewMode);
        }
        
        // Initialize time display
        updateTimeDisplay();
    }
    
    /**
     * Update the time display in the HUD
     */
    function updateTimeDisplay() {
        const timeDisplay = document.getElementById('time-display');
        const dayCounter = document.getElementById('day-counter');
        
        if (timeDisplay && window.timeLoopGame) {
            let timeText = '6:00 AM';
            
            // Set time based on current game time
            switch (window.timeLoopGame.currentTime) {
                case 'morning':
                    timeText = '6:00 AM';
                    break;
                case 'afternoon':
                    timeText = '1:00 PM';
                    break;
                case 'evening':
                    timeText = '6:00 PM';
                    break;
                case 'night':
                    timeText = '10:00 PM';
                    break;
            }
            
            timeDisplay.textContent = timeText;
        }
        
        if (dayCounter && window.timeLoopGame) {
            dayCounter.textContent = `Day ${window.timeLoopGame.currentDay}`;
        }
    }
    
    /**
     * Set up event listeners for buttons
     */
    function setupEventListeners() {
        // HUD buttons
        const journalButton = document.getElementById('journal-button');
        const inventoryButton = document.getElementById('inventory-button');
        const menuButton = document.getElementById('menu-button');
        
        if (journalButton) {
            journalButton.addEventListener('click', toggleJournal);
        }
        
        if (inventoryButton) {
            inventoryButton.addEventListener('click', toggleInventory);
        }
        
        if (menuButton) {
            menuButton.addEventListener('click', toggleMenu);
        }
        
        // Guide button
        const guideButton = document.getElementById('guide-button');
        if (guideButton) {
            guideButton.addEventListener('click', showGuide);
        }
    }
    
    /**
     * Toggle between 2D and 3D views
     */
    function toggleViewMode() {
        const imageView = document.getElementById('image-view');
        const scene3dContainer = document.getElementById('scene3d-container');
        const toggleButton = document.getElementById('toggle-view-mode');
        
        if (imageView && scene3dContainer) {
            const is3DMode = imageView.style.display === 'none';
            
            if (is3DMode) {
                // Switch to 2D
                imageView.style.display = 'flex';
                scene3dContainer.style.display = 'none';
                
                if (toggleButton) {
                    toggleButton.textContent = 'Switch to 3D View';
                }
            } else {
                // Switch to 3D
                imageView.style.display = 'none';
                scene3dContainer.style.display = 'block';
                
                // Initialize 3D scene if needed
                if (typeof initialize3DScene === 'function') {
                    initialize3DScene();
                } else {
                    // Fallback if 3D is not implemented
                    const placeholderText = document.createElement('p');
                    placeholderText.textContent = '3D view not available in this location';
                    placeholderText.className = 'placeholder-text';
                    
                    scene3dContainer.innerHTML = '';
                    scene3dContainer.appendChild(placeholderText);
                }
                
                if (toggleButton) {
                    toggleButton.textContent = 'Switch to 2D View';
                }
            }
        }
    }
    
    /**
     * Update time display in the HUD
     */
    function updateTimeDisplay() {
        if (window.timeLoopGame) {
            window.timeLoopGame.updateTimeDisplay();
        }
    }
    
    /**
     * Toggle journal visibility
     */
    function toggleJournal() {
        console.log('Journal toggled');
        // Create notification
        const notification = document.createElement('div');
        notification.className = 'notification info';
        notification.textContent = 'Journal feature coming soon!';
        
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        container.appendChild(notification);
        
        // Fade out after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 3000);
    }
    
    /**
     * Toggle inventory visibility
     */
    function toggleInventory() {
        console.log('Inventory toggled');
        // Create notification
        const notification = document.createElement('div');
        notification.className = 'notification info';
        notification.textContent = 'Inventory feature coming soon!';
        
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        container.appendChild(notification);
        
        // Fade out after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 3000);
    }
    
    /**
     * Toggle menu visibility
     */
    function toggleMenu() {
        console.log('Menu toggled');
        // Create notification
        const notification = document.createElement('div');
        notification.className = 'notification info';
        notification.textContent = 'Menu feature coming soon!';
        
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        container.appendChild(notification);
        
        // Fade out after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 3000);
    }
    
    /**
     * Show game guide/help
     */
    function showGuide() {
        console.log('Guide button clicked');
        // Create notification
        const notification = document.createElement('div');
        notification.className = 'notification guide';
        notification.textContent = "Don't Panic! This guide will help you navigate Time Loop.";
        
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        container.appendChild(notification);
        
        // Fade out after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 8000);
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
