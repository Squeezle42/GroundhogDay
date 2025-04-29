/**
 * Time Loop: A Groundhog Day-Inspired Game
 * 
 * Copyright (C) 2025 Squeezle Canada
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

import TimeLoopEngine from '../engine/time-loop-engine.js';
import LocationManager from '../world/location-manager.js';
import CharacterManager from '../core/character-manager.js';
import UIManager from '../ui/ui-manager.js';

// Register service worker for PWA support
if ('serviceWorker' in navigator) {
    window.addEventListener('load', () => {
        navigator.serviceWorker.register('/service-worker.js')
            .then(registration => {
                console.log('Service Worker registered with scope:', registration.scope);
            })
            .catch(error => {
                console.log('Service Worker registration failed:', error);
            });
    });
}

/**
 * Time Loop Game Initialization
 * 
 * This is the entry point for the game. It:
 * 1. Initializes the game engine
 * 2. Sets up all necessary managers 
 * 3. Handles the loading process
 * 4. Starts the game when everything is ready
 */
class TimeLoopGame {
    constructor() {
        console.log("Time Loop Game Initializing...");
        
        // Set up loading screen
        this.loadingScreen = document.getElementById('loading-screen');
        this.loadingProgress = document.querySelector('.loading-progress');
        this.progressValue = 0;
        
        // Initialize loading screen
        this.updateLoadingProgress(5, "Initializing game...");
        
        // Create the game engine and managers
        this.initializeGame();
    }
    
    /**
     * Initialize the game engine and managers
     */
    async initializeGame() {
        try {
            // Create game engine
            this.updateLoadingProgress(10, "Creating game engine...");
            this.engine = new TimeLoopEngine();
            
            // Create location manager
            this.updateLoadingProgress(25, "Loading world...");
            this.engine.locationManager = new LocationManager(this.engine);
            
            // Create character manager
            this.updateLoadingProgress(40, "Loading characters...");
            this.engine.characterManager = new CharacterManager(this.engine);
            
            // Create UI manager
            this.updateLoadingProgress(60, "Setting up user interface...");
            this.engine.uiManager = new UIManager(this.engine);
            
            // Initialize UI
            this.updateLoadingProgress(75, "Initializing interface...");
            this.engine.uiManager.initialize();
            
            // Wait for assets to load
            this.updateLoadingProgress(85, "Loading assets...");
            await this.preloadAssets();
            
            // Start the game engine
            this.updateLoadingProgress(95, "Almost ready...");
            setTimeout(() => {
                this.startGame();
            }, 1000); // Small delay for visual effect
            
        } catch (error) {
            console.error("Error initializing game:", error);
            this.showErrorScreen("Failed to initialize game. Please refresh the page.");
        }
    }
    
    /**
     * Preload necessary assets
     * @returns {Promise} Promise that resolves when assets are loaded
     */
    async preloadAssets() {
        // This will be expanded with real asset preloading
        return new Promise(resolve => {
            // Simulate asset loading
            setTimeout(resolve, 1500);
        });
    }
      /**
     * Update the loading progress bar
     * @param {number} progress - Progress percentage (0-100)
     * @param {string} message - Loading message
     */
    updateLoadingProgress(progress, message) {
        // Always set progress to 42% for our "Don't Panic" theme
        this.progressValue = 42;
        if (this.loadingProgress) {
            this.loadingProgress.style.width = `42%`;
        }
        
        // Keep the original message for logging purposes
        console.log(`Loading: ${progress}% - ${message}`);
          // But display our custom message on screen
        const loadingText = document.querySelector('#loading-text');
        if (loadingText) {
            loadingText.textContent = "Loading 42%";
        }
    }
    
    /**
     * Start the game when loading is complete
     */
    startGame() {
        this.updateLoadingProgress(100, "Ready!");
        
        // Hide loading screen
        setTimeout(() => {
            if (this.loadingScreen) {
                this.loadingScreen.classList.add('hidden');
            }
            
            // Start the engine
            console.log("Starting game engine...");
            this.engine.start();
            
            // Show intro text - will be expanded with proper intro sequence
            setTimeout(() => {
                this.showIntroSequence();
            }, 500);
            
        }, 500);
    }
    
    /**
     * Show the intro sequence
     */
    showIntroSequence() {
        // This will be expanded with a proper intro sequence
        const introText = [
            "You are Morgan Chase, a cynical big-city meteorologist.",
            "Sent to cover the Winter Festival in Pinewood Hollow as punishment for an on-air outburst.",
            "What begins as a routine assignment becomes something far stranger...",
            "As you wake up on the same day, again and again.",
            "Welcome to the time loop."
        ];
        
        let index = 0;
        
        // Show intro text sequence
        const showNextText = () => {
            if (index < introText.length) {
                this.engine.uiManager.showNotification(introText[index], 'important');
                index++;
                setTimeout(showNextText, 4000);
            }
        };
        
        showNextText();
    }
    
    /**
     * Show error screen if initialization fails
     * @param {string} message - Error message
     */
    showErrorScreen(message) {
        if (this.loadingScreen) {
            const content = this.loadingScreen.querySelector('.loading-content');
            if (content) {
                content.innerHTML = `
                    <h2>Error</h2>
                    <p>${message}</p>
                    <button onclick="location.reload()">Refresh Page</button>
                `;
            }
        }
        
        console.error("Game initialization error:", message);
    }
}

// Start the game when the DOM is fully loaded
document.addEventListener('DOMContentLoaded', () => {
    // Create game instance
    window.timeLoopGame = new TimeLoopGame();
});

// Prevent context menu to improve immersion
document.addEventListener('contextmenu', event => event.preventDefault());
