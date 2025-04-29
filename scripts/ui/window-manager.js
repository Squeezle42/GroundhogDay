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
 * WindowManager - Manages the application window settings and behaviors
 */
class WindowManager {
    constructor() {
        // Initialize properties
        this.isFullScreen = false;
        this.originalWidth = window.innerWidth;
        this.originalHeight = window.innerHeight;
        
        // Bind events
        this.bindEvents();
        
        // Set up guide button
        this.setupGuideButton();
        
        console.log("WindowManager initialized");
    }
    
    /**
     * Bind event listeners
     */
    bindEvents() {
        // Listen for resize events
        window.addEventListener('resize', this.handleResize.bind(this));
        
        // Listen for fullscreen change events
        document.addEventListener('fullscreenchange', this.handleFullscreenChange.bind(this));
        document.addEventListener('webkitfullscreenchange', this.handleFullscreenChange.bind(this));
        document.addEventListener('mozfullscreenchange', this.handleFullscreenChange.bind(this));
        document.addEventListener('MSFullscreenChange', this.handleFullscreenChange.bind(this));
        
        // Listen for keyboard shortcuts
        window.addEventListener('keydown', this.handleKeydown.bind(this));
    }
    
    /**
     * Set up the guide button (42) to open the README.md with installation instructions
     */
    setupGuideButton() {
        const guideButton = document.getElementById('guide-button');
        if (guideButton) {
            guideButton.addEventListener('click', () => {
                this.openReadme();
            });
        }
    }    /**
     * Open the README.md file with installation instructions
     */
    openReadme() {
        // Check if we're in an installed app context
        const isInstalledApp = window.matchMedia('(display-mode: standalone)').matches || 
                              window.navigator.standalone === true;
        
        if (isInstalledApp) {
            // In installed app context, show an overlay
            this.showHelpOverlay();
        } else {
            // In a web context, open README-InApp.md in a new tab
            window.open('README-InApp.md', '_blank');
            
            // Display a notification
            this.showNotification("Opening installation instructions...");
        }
    }
    
    /**
     * Show an in-app help overlay with README content
     */
    showHelpOverlay() {
        // Create overlay if it doesn't exist
        let helpOverlay = document.getElementById('help-overlay');
        
        if (!helpOverlay) {
            helpOverlay = document.createElement('div');
            helpOverlay.id = 'help-overlay';
            helpOverlay.className = 'overlay';
            
            const overlayContent = document.createElement('div');
            overlayContent.className = 'overlay-content';
            
            // Add close button
            const closeButton = document.createElement('button');
            closeButton.className = 'overlay-close-button';
            closeButton.innerHTML = '×';
            closeButton.addEventListener('click', () => {
                helpOverlay.classList.add('hidden');
            });
            
            // Add title
            const title = document.createElement('h2');
            title.textContent = 'Time Loop - Installation & Help';
            
            // Create content container with scroll
            const contentContainer = document.createElement('div');
            contentContainer.className = 'overlay-scroll-container';
            
            // Add main content sections
            contentContainer.innerHTML = `
                <div class="help-section">
                    <h3>Installation Options</h3>
                    <h4>Desktop Application (Recommended)</h4>
                    <ul>
                        <li><strong>Windows:</strong> Run TimeLoop-Setup-Windows.exe</li>
                        <li><strong>macOS:</strong> Open TimeLoop-macOS.dmg and drag to Applications</li>
                        <li><strong>Linux:</strong> Make timeloop-linux.AppImage executable and run it</li>
                    </ul>
                    
                    <h4>Web Version</h4>
                    <p>Visit <a href="https://play.timeloop-game.example.com" target="_blank">play.timeloop-game.example.com</a></p>
                </div>
                
                <div class="help-section">
                    <h3>Controls</h3>
                    <ul>
                        <li><strong>WASD or Arrow Keys:</strong> Move character</li>
                        <li><strong>Mouse:</strong> Look around / Select options</li>
                        <li><strong>E or Left Click:</strong> Interact with objects</li>
                        <li><strong>Tab:</strong> Open journal</li>
                        <li><strong>I:</strong> Open inventory</li>
                        <li><strong>Esc:</strong> Pause menu / Back</li>
                        <li><strong>F11 or Alt+Enter:</strong> Toggle fullscreen</li>
                        <li><strong>F1 or ?:</strong> Open this help screen</li>
                        <li><strong>End:</strong> Skip to ending (Demo mode only)</li>
                    </ul>
                </div>
                
                <div class="help-section">
                    <h3>Troubleshooting</h3>
                    <ul>
                        <li><strong>Game won't start:</strong> Ensure your browser is up-to-date</li>
                        <li><strong>Performance issues:</strong> Close other applications</li>
                        <li><strong>Black screen:</strong> Try refreshing or restarting</li>
                    </ul>
                </div>
                
                <div class="help-section">
                    <h3>About</h3>
                    <p>Time Loop is a narrative adventure inspired by "Groundhog Day" where you're trapped in a repeating day.</p>
                    <p><strong>Version:</strong> 0.1.0 (Early Development)</p>
                    <p><strong>Support:</strong> support@timeloop-game.example.com</p>
                </div>
            `;
            
            // Add keyboard shortcuts to close
            document.addEventListener('keydown', (event) => {
                if (event.key === 'Escape' && !helpOverlay.classList.contains('hidden')) {
                    helpOverlay.classList.add('hidden');
                }
            });
            
            // Assemble all elements
            overlayContent.appendChild(closeButton);
            overlayContent.appendChild(title);
            overlayContent.appendChild(contentContainer);
            helpOverlay.appendChild(overlayContent);
            document.body.appendChild(helpOverlay);
        }
        
        // Show the overlay
        helpOverlay.classList.remove('hidden');
        
        // Display a notification
        this.showNotification("Showing installation instructions");
    }
    
    /**
     * Handle window resize events
     * @param {Event} event - The resize event
     */
    handleResize(event) {
        // Update UI elements that need to be responsive
        console.log(`Window resized to ${window.innerWidth}x${window.innerHeight}`);
        
        // You can add custom behavior here, like adjusting canvas size or UI elements
    }
    
    /**
     * Toggle fullscreen mode
     */
    toggleFullscreen() {
        if (!this.isFullScreen) {
            this.enterFullscreen();
        } else {
            this.exitFullscreen();
        }
    }
    
    /**
     * Enter fullscreen mode
     */
    enterFullscreen() {
        const elem = document.documentElement;
        
        if (elem.requestFullscreen) {
            elem.requestFullscreen();
        } else if (elem.mozRequestFullScreen) { /* Firefox */
            elem.mozRequestFullScreen();
        } else if (elem.webkitRequestFullscreen) { /* Chrome, Safari & Opera */
            elem.webkitRequestFullscreen();
        } else if (elem.msRequestFullscreen) { /* IE/Edge */
            elem.msRequestFullscreen();
        }
    }
    
    /**
     * Exit fullscreen mode
     */
    exitFullscreen() {
        if (document.exitFullscreen) {
            document.exitFullscreen();
        } else if (document.mozCancelFullScreen) { /* Firefox */
            document.mozCancelFullScreen();
        } else if (document.webkitExitFullscreen) { /* Chrome, Safari & Opera */
            document.webkitExitFullscreen();
        } else if (document.msExitFullscreen) { /* IE/Edge */
            document.msExitFullscreen();
        }
    }
    
    /**
     * Handle fullscreen change events
     * @param {Event} event - The fullscreen change event
     */
    handleFullscreenChange(event) {
        // Check if we're in fullscreen mode
        this.isFullScreen = document.fullscreenElement || 
                           document.webkitFullscreenElement || 
                           document.mozFullScreenElement ||
                           document.msFullscreenElement;
        
        console.log(`Fullscreen mode: ${this.isFullScreen ? 'Enabled' : 'Disabled'}`);
        
        // You can add custom behavior here, like adjusting UI for fullscreen
    }
    
    /**
     * Handle keyboard shortcuts
     * @param {KeyboardEvent} event - The keyboard event
     */
    handleKeydown(event) {
        // F11 or Alt+Enter to toggle fullscreen
        if (event.key === 'F11' || (event.altKey && event.key === 'Enter')) {
            event.preventDefault();
            this.toggleFullscreen();
        }
        
        // Escape to exit fullscreen
        if (event.key === 'Escape' && this.isFullScreen) {
            this.exitFullscreen();
        }
        
        // F1 or ? to open README/help
        if (event.key === 'F1' || event.key === '?') {
            event.preventDefault();
            this.openReadme();
        }
    }
    
    /**
     * Show a notification message
     * @param {string} message - The message to display
     */
    showNotification(message) {
        // Check if notification container exists, if not create it
        let notificationContainer = document.getElementById('notification-container');
        
        if (!notificationContainer) {
            notificationContainer = document.createElement('div');
            notificationContainer.id = 'notification-container';
            document.body.appendChild(notificationContainer);
        }
        
        // Create notification element
        const notification = document.createElement('div');
        notification.className = 'notification';
        notification.textContent = message;
        
        // Add to container
        notificationContainer.appendChild(notification);
        
        // Remove after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => {
                notificationContainer.removeChild(notification);
            }, 500);
        }, 3000);
    }
}

// Initialize when the DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.windowManager = new WindowManager();
});
