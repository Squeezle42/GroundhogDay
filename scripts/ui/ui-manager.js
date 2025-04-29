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
 * UIManager - Handles all user interface elements and interactions
 * Manages the game's visual display, menus, and input handling
 */
class UIManager {
    /**
     * Create a new UIManager
     * @param {Object} gameEngine - Reference to main game engine
     */
    constructor(gameEngine) {
        this.engine = gameEngine;
        
        // UI elements
        this.elements = {
            locationTitle: document.getElementById('location-title'),
            locationDesc: document.getElementById('location-description'),
            timeDisplay: document.getElementById('time-display'),
            dayCounter: document.getElementById('day-counter'),
            weatherDisplay: document.getElementById('weather-display'),
            mainContent: document.getElementById('main-content'),
            locationImage: document.getElementById('location-image'),
            interactionArea: document.getElementById('interaction-area'),
            dialogueBox: document.getElementById('dialogue-box'),
            navigationButtons: document.getElementById('navigation-buttons'),
            interactionButtons: document.getElementById('interaction-buttons'),
            journalButton: document.getElementById('journal-button'),
            inventoryButton: document.getElementById('inventory-button'),
            menuButton: document.getElementById('menu-button')
        };
        
        // UI state
        this.activeScreen = 'main'; // main, dialogue, journal, inventory, menu
        this.activeDialogue = null;
        this.notifications = [];
        
        // Set up event handlers
        this.setupEvents();
        
        console.log("UI Manager initialized");
    }
    
    /**
     * Set up event listeners for UI events
     */
    setupEvents() {
        // Game state events
        document.addEventListener('locationChange', this.onLocationChange.bind(this));
        document.addEventListener('hourChanged', this.onTimeChanged.bind(this));
        document.addEventListener('weatherChanged', this.onWeatherChanged.bind(this));
        document.addEventListener('dayReset', this.onDayReset.bind(this));
        
        // Interaction events
        document.addEventListener('dialogueStart', this.onDialogueStart.bind(this));
        document.addEventListener('dialogueEnd', this.onDialogueEnd.bind(this));
        
        // Notification events
        document.addEventListener('notification', this.onNotification.bind(this));
        document.addEventListener('questUpdate', this.onQuestUpdate.bind(this));
        document.addEventListener('playerGrowth', this.onPlayerGrowth.bind(this));
        
        // Button events
        this.setupButtonListeners();
        
        console.log("UI events initialized");
    }
    
    /**
     * Set up button click listeners
     */
    setupButtonListeners() {
        // Navigation menu toggle
        if (this.elements.menuButton) {
            this.elements.menuButton.addEventListener('click', () => {
                this.toggleScreen('menu');
            });
        }
        
        // Journal button
        if (this.elements.journalButton) {
            this.elements.journalButton.addEventListener('click', () => {
                this.toggleScreen('journal');
            });
        }
        
        // Inventory button
        if (this.elements.inventoryButton) {
            this.elements.inventoryButton.addEventListener('click', () => {
                this.toggleScreen('inventory');
            });
        }
    }
    
    /**
     * Initialize the UI
     */
    initialize() {
        // Initial UI update
        this.updateTimeDisplay();
        this.updateDayCounter();
        this.updateWeatherDisplay();
        
        console.log("UI initialized");
    }
    
    /**
     * Render the game UI
     * @param {Object} gameState - Current game state
     */
    render(gameState) {
        // Update time and day displays
        this.updateTimeDisplay();
        this.updateDayCounter();
        
        // Only update location display if it's the main screen
        if (this.activeScreen === 'main') {
            this.updateLocationDisplay();
        }
        
        // Process any pending notifications
        this.processPendingNotifications();
    }
    
    /**
     * Update the time display
     */
    updateTimeDisplay() {
        if (!this.elements.timeDisplay) return;
        
        const time = this.engine.formatGameTime();
        this.elements.timeDisplay.textContent = time;
    }
    
    /**
     * Update the day counter
     */
    updateDayCounter() {
        if (!this.elements.dayCounter) return;
        
        const day = this.engine.gameState.currentDay;
        this.elements.dayCounter.textContent = `Day ${day}`;
    }
    
    /**
     * Update the weather display
     */
    updateWeatherDisplay() {
        if (!this.elements.weatherDisplay) return;
        
        const weather = this.engine.gameState.weather;
        this.elements.weatherDisplay.textContent = weather.charAt(0).toUpperCase() + weather.slice(1);
        
        // Apply weather-specific CSS class
        this.elements.weatherDisplay.className = `weather ${weather}`;
    }
    
    /**
     * Update the location display
     */
    updateLocationDisplay() {
        // Get current location data from location manager
        const locationData = this.engine.locationManager?.getCurrentLocationData();
        if (!locationData) return;
        
        // Update location title and description
        if (this.elements.locationTitle) {
            this.elements.locationTitle.textContent = locationData.name;
        }
        
        if (this.elements.locationDesc) {
            this.elements.locationDesc.textContent = locationData.description;
        }
        
        // Update location image
        if (this.elements.locationImage) {
            const imagePath = this.getLocationImagePath(locationData.id);
            this.elements.locationImage.src = imagePath;
            this.elements.locationImage.alt = locationData.name;
        }
        
        // Update navigation buttons
        this.updateNavigationButtons(locationData.connections);
        
        // Update interaction buttons
        this.updateInteractionButtons(locationData.objects);
        
        // Add NPCs in this location
        this.updateNPCDisplay();
    }
    
    /**
     * Get the image path for a location
     * @param {string} locationId - The location ID
     * @returns {string} The image path
     */
    getLocationImagePath(locationId) {
        const time = this.getTimeOfDay();
        const weather = this.engine.gameState.weather;
        
        return `assets/images/locations/loc_${locationId}_${time}_${weather}.jpg`;
    }
    
    /**
     * Get the current time of day period (morning, day, evening, night)
     * @returns {string} Time of day period
     */
    getTimeOfDay() {
        const hour = this.engine.gameState.timeOfDay;
        
        if (hour >= 5 && hour < 10) return 'morning';
        if (hour >= 10 && hour < 17) return 'day';
        if (hour >= 17 && hour < 21) return 'evening';
        return 'night';
    }
    
    /**
     * Update the navigation buttons
     * @param {Array} connections - Available connections from current location
     */
    updateNavigationButtons(connections) {
        if (!this.elements.navigationButtons) return;
        
        // Clear existing buttons
        this.elements.navigationButtons.innerHTML = '';
        
        // Add a button for each connection
        connections.forEach(connection => {
            const button = document.createElement('button');
            button.textContent = connection.name;
            button.classList.add('nav-button');
            
            // Add disabled class if location is not accessible
            if (!connection.accessible) {
                button.classList.add('disabled');
                button.title = "This location is not accessible right now";
            }
            
            // Add click event
            button.addEventListener('click', () => {
                if (connection.accessible) {
                    this.engine.locationManager.moveToLocation(connection.id);
                } else {
                    this.showNotification(`You cannot go to ${connection.name} right now.`);
                }
            });
            
            this.elements.navigationButtons.appendChild(button);
        });
    }
    
    /**
     * Update the interaction buttons
     * @param {Array} objects - Interactable objects in the current location
     */
    updateInteractionButtons(objects) {
        if (!this.elements.interactionButtons) return;
        
        // Clear existing buttons
        this.elements.interactionButtons.innerHTML = '';
        
        // Add a button for each interactable object
        objects.forEach(object => {
            const button = document.createElement('button');
            button.textContent = object.id.replace(/_/g, ' ');
            button.classList.add('interaction-button');
            
            // Add click event
            button.addEventListener('click', () => {
                const result = this.engine.locationManager.interactWithObject(object.id);
                
                if (result.success) {
                    this.showNotification(result.message);
                    
                    // If the interaction resulted in a discovery
                    if (result.discovery) {
                        this.showDiscovery(result.discovery);
                    }
                }
            });
            
            this.elements.interactionButtons.appendChild(button);
        });
        
        // Add NPCs as interactable "objects"
        const npcs = this.engine.characterManager?.getNPCsAtLocation(
            this.engine.gameState.playerLocation
        ) || [];
        
        npcs.forEach(npc => {
            const button = document.createElement('button');
            button.textContent = `Talk to ${npc.name}`;
            button.classList.add('interaction-button', 'npc-button');
            
            // Add click event
            button.addEventListener('click', () => {
                const result = this.engine.characterManager.interactWithCharacter(npc.id);
                
                if (result.success) {
                    // Start dialogue
                    document.dispatchEvent(new CustomEvent('dialogueStart', {
                        detail: result
                    }));
                } else {
                    this.showNotification(result.message);
                }
            });
            
            this.elements.interactionButtons.appendChild(button);
        });
    }
    
    /**
     * Update the display of NPCs in the current location
     */
    updateNPCDisplay() {
        // This will be expanded with visual NPC representations
        const npcs = this.engine.characterManager?.getNPCsAtLocation(
            this.engine.gameState.playerLocation
        ) || [];
        
        // For now, just log to console
        console.log(`NPCs at current location: ${npcs.map(npc => npc.name).join(', ')}`);
    }
    
    /**
     * Handle location change event
     * @param {CustomEvent} event - The location change event
     */
    onLocationChange(event) {
        // Update UI for new location
        this.updateLocationDisplay();
    }
    
    /**
     * Handle time change event
     * @param {CustomEvent} event - The time change event
     */
    onTimeChanged(event) {
        // Update time display
        this.updateTimeDisplay();
        
        // Update location to reflect time changes (e.g., lighting)
        this.updateLocationDisplay();
    }
    
    /**
     * Handle weather change event
     * @param {CustomEvent} event - The weather change event
     */
    onWeatherChanged(event) {
        // Update weather display
        this.updateWeatherDisplay();
        
        // Update location image to reflect weather
        this.updateLocationDisplay();
        
        // Show notification about weather change
        this.showNotification(`The weather has changed to ${event.detail.weather}.`);
    }
    
    /**
     * Handle day reset event
     * @param {CustomEvent} event - The day reset event
     */
    onDayReset(event) {
        // Update day counter
        this.updateDayCounter();
        
        // Show day reset notification
        this.showNotification(`You wake up at the beginning of day ${event.detail.day}.`, 'important');
        
        // Update all displays
        this.updateTimeDisplay();
        this.updateWeatherDisplay();
        this.updateLocationDisplay();
    }
    
    /**
     * Handle dialogue start event
     * @param {CustomEvent} event - The dialogue start event
     */
    onDialogueStart(event) {
        // Switch to dialogue screen
        this.activeScreen = 'dialogue';
        this.activeDialogue = event.detail;
        
        // Show dialogue UI
        this.showDialogueScreen();
    }
    
    /**
     * Handle dialogue end event
     * @param {CustomEvent} event - The dialogue end event
     */
    onDialogueEnd(event) {
        // Switch back to main screen
        this.activeScreen = 'main';
        this.activeDialogue = null;
        
        // Hide dialogue UI
        this.hideDialogueScreen();
        
        // Update main screen
        this.updateLocationDisplay();
    }
    
    /**
     * Show the dialogue screen
     */
    showDialogueScreen() {
        if (!this.elements.dialogueBox || !this.activeDialogue) return;
        
        // Show dialogue box
        this.elements.dialogueBox.style.display = 'block';
        
        // Hide main interaction area
        if (this.elements.interactionArea) {
            this.elements.interactionArea.style.display = 'none';
        }
        
        // Populate dialogue content
        this.updateDialogueContent();
    }
    
    /**
     * Hide the dialogue screen
     */
    hideDialogueScreen() {
        if (!this.elements.dialogueBox) return;
        
        // Hide dialogue box
        this.elements.dialogueBox.style.display = 'none';
        
        // Show main interaction area
        if (this.elements.interactionArea) {
            this.elements.interactionArea.style.display = 'block';
        }
    }
    
    /**
     * Update the dialogue content
     */
    updateDialogueContent() {
        // This will be expanded with real dialogue system integration
        if (!this.elements.dialogueBox || !this.activeDialogue) return;
        
        // For now, create a simple dialogue display
        this.elements.dialogueBox.innerHTML = `
            <div class="dialogue-header">
                <h3>${this.activeDialogue.characterName}</h3>
                <p class="activity">${this.activeDialogue.characterActivity}</p>
            </div>
            <div class="dialogue-content">
                <p>Hello there! This is placeholder dialogue for ${this.activeDialogue.characterName}.</p>
                <p>Character is currently ${this.activeDialogue.characterActivity}.</p>
            </div>
            <div class="dialogue-options">
                <button id="dialogue-option-1">Ask about the town</button>
                <button id="dialogue-option-2">Ask about yourself</button>
                <button id="dialogue-option-3">End conversation</button>
            </div>
        `;
        
        // Add event listeners to dialogue options
        document.getElementById('dialogue-option-1').addEventListener('click', () => {
            // Replace with real dialogue system
            this.showNotification(`You ask ${this.activeDialogue.characterName} about the town.`);
        });
        
        document.getElementById('dialogue-option-2').addEventListener('click', () => {
            // Replace with real dialogue system
            this.showNotification(`You ask ${this.activeDialogue.characterName} about themselves.`);
        });
        
        document.getElementById('dialogue-option-3').addEventListener('click', () => {
            // End dialogue
            document.dispatchEvent(new CustomEvent('dialogueEnd', {
                detail: {
                    character: this.activeDialogue.character,
                    outcome: { ended: true }
                }
            }));
        });
    }
    
    /**
     * Handle notification event
     * @param {CustomEvent} event - The notification event
     */
    onNotification(event) {
        const { message, type } = event.detail;
        this.showNotification(message, type);
    }
    
    /**
     * Handle quest update event
     * @param {CustomEvent} event - The quest update event
     */
    onQuestUpdate(event) {
        const { quest, status, progress } = event.detail;
        
        // Show quest update notification
        if (status === 'started') {
            this.showNotification(`New quest: ${quest}`, 'quest');
        } else if (status === 'completed') {
            this.showNotification(`Quest completed: ${quest}`, 'quest-complete');
        } else if (status === 'updated') {
            this.showNotification(`Quest updated: ${quest}`, 'quest-update');
        }
        
        // Update journal if it's open
        if (this.activeScreen === 'journal') {
            this.updateJournalDisplay();
        }
    }
    
    /**
     * Handle player growth event
     * @param {CustomEvent} event - The player growth event
     */
    onPlayerGrowth(event) {
        const { type, message } = event.detail;
        
        // Show growth notification
        this.showNotification(message, 'growth');
        
        // Update character screen if it's open
        if (this.activeScreen === 'character') {
            this.updateCharacterDisplay();
        }
    }
    
    /**
     * Show a notification message
     * @param {string} message - The message to show
     * @param {string} type - The notification type (default, important, quest, etc.)
     */
    showNotification(message, type = 'default') {
        // Add notification to queue
        this.notifications.push({ message, type, time: Date.now() });
        
        // Process pending notifications
        this.processPendingNotifications();
    }
    
    /**
     * Process any pending notifications
     */
    processPendingNotifications() {
        // If no notifications, do nothing
        if (this.notifications.length === 0) return;
        
        // Get notification container or create one
        let notificationContainer = document.getElementById('notification-container');
        
        if (!notificationContainer) {
            notificationContainer = document.createElement('div');
            notificationContainer.id = 'notification-container';
            document.body.appendChild(notificationContainer);
        }
        
        // Process next notification
        const notification = this.notifications.shift();
        
        // Create notification element
        const notificationElement = document.createElement('div');
        notificationElement.className = `notification ${notification.type}`;
        notificationElement.textContent = notification.message;
        
        // Add to container
        notificationContainer.appendChild(notificationElement);
        
        // Remove after animation
        setTimeout(() => {
            notificationElement.classList.add('fade-out');
            setTimeout(() => {
                notificationContainer.removeChild(notificationElement);
            }, 500);
        }, 3000);
    }
    
    /**
     * Show a discovery notification
     * @param {string} discovery - The discovery ID
     */
    showDiscovery(discovery) {
        // This will be expanded with more specific discovery handling
        let message = "You've made a discovery!";
        
        // Custom messages for specific discoveries
        if (discovery === 'hidden_valley_path') {
            message = "You've discovered a path to Hidden Valley!";
        }
        
        // Show discovery notification
        this.showNotification(message, 'discovery');
        
        // Add discovery sound
        this.playSound('discovery');
    }
    
    /**
     * Toggle between different UI screens
     * @param {string} screen - The screen to toggle (main, journal, inventory, menu, character)
     */
    toggleScreen(screen) {
        // If already on this screen, go back to main
        if (this.activeScreen === screen) {
            this.activeScreen = 'main';
            this.updateLocationDisplay();
            return;
        }
        
        this.activeScreen = screen;
        
        // Update UI based on active screen
        switch (screen) {
            case 'main':
                this.updateLocationDisplay();
                break;
                
            case 'journal':
                this.showJournalScreen();
                break;
                
            case 'inventory':
                this.showInventoryScreen();
                break;
                
            case 'menu':
                this.showMenuScreen();
                break;
                
            case 'character':
                this.showCharacterScreen();
                break;
                
            default:
                console.error(`Unknown screen: ${screen}`);
                this.activeScreen = 'main';
                this.updateLocationDisplay();
        }
    }
    
    /**
     * Show the journal screen
     */
    showJournalScreen() {
        if (!this.elements.mainContent) return;
        
        // Set content for journal screen
        this.elements.mainContent.innerHTML = `
            <div class="journal-screen">
                <h2>Morgan's Journal</h2>
                <div class="journal-tabs">
                    <button class="journal-tab active" data-tab="quests">Quests</button>
                    <button class="journal-tab" data-tab="knowledge">Knowledge</button>
                    <button class="journal-tab" data-tab="characters">Characters</button>
                    <button class="journal-tab" data-tab="locations">Locations</button>
                </div>
                <div class="journal-content">
                    <div id="quests-content" class="tab-content active"></div>
                    <div id="knowledge-content" class="tab-content"></div>
                    <div id="characters-content" class="tab-content"></div>
                    <div id="locations-content" class="tab-content"></div>
                </div>
                <button id="close-journal" class="close-button">Close Journal</button>
            </div>
        `;
        
        // Add content to each tab
        this.updateJournalDisplay();
        
        // Add tab switching logic
        const tabs = document.querySelectorAll('.journal-tab');
        tabs.forEach(tab => {
            tab.addEventListener('click', () => {
                // Deactivate all tabs
                tabs.forEach(t => t.classList.remove('active'));
                document.querySelectorAll('.tab-content').forEach(
                    c => c.classList.remove('active')
                );
                
                // Activate selected tab
                tab.classList.add('active');
                const tabName = tab.getAttribute('data-tab');
                document.getElementById(`${tabName}-content`).classList.add('active');
            });
        });
        
        // Add close button handler
        document.getElementById('close-journal').addEventListener('click', () => {
            this.toggleScreen('main');
        });
    }
    
    /**
     * Update the journal display content
     */
    updateJournalDisplay() {
        // Quests tab
        const questsContent = document.getElementById('quests-content');
        if (questsContent) {
            questsContent.innerHTML = this.generateQuestsContent();
        }
        
        // Knowledge tab
        const knowledgeContent = document.getElementById('knowledge-content');
        if (knowledgeContent) {
            knowledgeContent.innerHTML = this.generateKnowledgeContent();
        }
        
        // Characters tab
        const charactersContent = document.getElementById('characters-content');
        if (charactersContent) {
            charactersContent.innerHTML = this.generateCharactersContent();
        }
        
        // Locations tab
        const locationsContent = document.getElementById('locations-content');
        if (locationsContent) {
            locationsContent.innerHTML = this.generateLocationsContent();
        }
    }
    
    /**
     * Generate content for the quests tab
     * @returns {string} HTML content
     */
    generateQuestsContent() {
        const questStatus = this.engine.gameState.questStatus;
        
        // If no quests
        if (Object.keys(questStatus).length === 0) {
            return '<p class="empty-content">No quests discovered yet.</p>';
        }
        
        let content = '<div class="quest-list">';
        
        for (const [quest, status] of Object.entries(questStatus)) {
            content += `
                <div class="quest-item ${status.status}">
                    <h3>${quest.replace(/_/g, ' ')}</h3>
                    <p>Status: ${status.status}</p>
                    <div class="quest-progress">
                        <div class="progress-bar" style="width: ${status.progress}%"></div>
                    </div>
                </div>
            `;
        }
        
        content += '</div>';
        return content;
    }
    
    /**
     * Generate content for the knowledge tab
     * @returns {string} HTML content
     */
    generateKnowledgeContent() {
        const knowledge = this.engine.gameState.knowledgeBase;
        
        // If no knowledge
        if (Object.keys(knowledge).length === 0) {
            return '<p class="empty-content">No knowledge recorded yet.</p>';
        }
        
        let content = '<div class="knowledge-categories">';
        
        // Group knowledge by category based on key prefixes
        const categories = {
            'Character Secrets': Object.entries(knowledge).filter(([key]) => key.startsWith('secret_')),
            'Locations': Object.entries(knowledge).filter(([key]) => key.includes('Location')),
            'Town History': Object.entries(knowledge).filter(([key]) => key.includes('history')),
            'Clock Tower': Object.entries(knowledge).filter(([key]) => key.includes('clock')),
            'Other': Object.entries(knowledge).filter(([key]) => 
                !key.startsWith('secret_') && 
                !key.includes('Location') && 
                !key.includes('history') && 
                !key.includes('clock')
            )
        };
        
        for (const [category, items] of Object.entries(categories)) {
            if (items.length === 0) continue;
            
            content += `<div class="knowledge-category"><h3>${category}</h3><ul>`;
            
            items.forEach(([key, value]) => {
                // Format the knowledge entry
                const formattedKey = key.replace(/_/g, ' ')
                    .replace(/secret /g, '')
                    .replace(/Location/g, 'location');
                
                content += `<li><strong>${formattedKey}:</strong> ${value}</li>`;
            });
            
            content += '</ul></div>';
        }
        
        content += '</div>';
        return content;
    }
    
    /**
     * Generate content for the characters tab
     * @returns {string} HTML content
     */
    generateCharactersContent() {
        const relationships = this.engine.gameState.relationships;
        const characters = this.engine.characterManager?.characters || {};
        
        // If no character relationships
        if (Object.keys(relationships).length === 0) {
            return '<p class="empty-content">No character relationships developed yet.</p>';
        }
        
        let content = '<div class="character-list">';
        
        for (const [characterId, relationshipValue] of Object.entries(relationships)) {
            // Get character data if available
            const character = characters[characterId];
            const name = character ? character.name : characterId.replace(/_/g, ' ');
            
            // Determine relationship status
            let relationshipStatus;
            if (relationshipValue <= 0) relationshipStatus = 'negative';
            else if (relationshipValue < 10) relationshipStatus = 'neutral';
            else if (relationshipValue < 25) relationshipStatus = 'positive';
            else relationshipStatus = 'strong';
            
            content += `
                <div class="character-item">
                    <h3>${name}</h3>
                    <div class="relationship-meter ${relationshipStatus}">
                        <div class="meter-fill" style="width: ${Math.min(100, Math.max(0, relationshipValue) * 4)}%"></div>
                    </div>
                    <p class="relationship-label">${this.getRelationshipLabel(relationshipValue)}</p>
                </div>
            `;
        }
        
        content += '</div>';
        return content;
    }
    
    /**
     * Get a text label for a relationship value
     * @param {number} value - The relationship value
     * @returns {string} Relationship description
     */
    getRelationshipLabel(value) {
        if (value <= -20) return "Hostile";
        if (value <= -10) return "Annoyed";
        if (value < 0) return "Unfriendly";
        if (value === 0) return "Neutral";
        if (value < 10) return "Acquaintance";
        if (value < 20) return "Friendly";
        if (value < 30) return "Trusted";
        return "Close";
    }
    
    /**
     * Generate content for the locations tab
     * @returns {string} HTML content
     */
    generateLocationsContent() {
        const knowledge = this.engine.gameState.knowledgeBase;
        const locations = this.engine.locationManager?.locations || {};
        
        let discoveredLocations = Object.keys(locations).filter(locationId => {
            // Check if location has been visited or discovered through knowledge
            return locations[locationId].state?.discovered || 
                   knowledge[`${locationId}Location`] ||
                   locationId === 'inn' || // Starting location
                   locationId === 'town_square'; // Central location
        });
        
        // If no locations discovered
        if (discoveredLocations.length === 0) {
            return '<p class="empty-content">No locations discovered yet.</p>';
        }
        
        let content = '<div class="location-list">';
        
        discoveredLocations.forEach(locationId => {
            const location = locations[locationId];
            
            content += `
                <div class="location-item">
                    <h3>${location.name}</h3>
                    <p>${location.description}</p>
                </div>
            `;
        });
        
        content += '</div>';
        return content;
    }
    
    /**
     * Show the inventory screen
     */
    showInventoryScreen() {
        if (!this.elements.mainContent) return;
        
        // Set content for inventory screen
        this.elements.mainContent.innerHTML = `
            <div class="inventory-screen">
                <h2>Inventory</h2>
                <div id="inventory-items" class="inventory-items"></div>
                <button id="close-inventory" class="close-button">Close Inventory</button>
            </div>
        `;
        
        // Add inventory items
        this.updateInventoryDisplay();
        
        // Add close button handler
        document.getElementById('close-inventory').addEventListener('click', () => {
            this.toggleScreen('main');
        });
    }
    
    /**
     * Update the inventory display
     */
    updateInventoryDisplay() {
        const inventoryItems = document.getElementById('inventory-items');
        if (!inventoryItems) return;
        
        const items = this.engine.gameState.inventory;
        
        // If inventory is empty
        if (items.length === 0) {
            inventoryItems.innerHTML = '<p class="empty-content">Your inventory is empty.</p>';
            return;
        }
        
        let content = '';
        
        items.forEach(item => {
            content += `
                <div class="inventory-item" data-item-id="${item.id}">
                    <img src="assets/images/items/item_${item.id}_inventory.png" alt="${item.name}">
                    <div class="item-details">
                        <h3>${item.name}</h3>
                        <p>${item.description}</p>
                        <button class="item-action-button use-button">Use</button>
                        <button class="item-action-button examine-button">Examine</button>
                    </div>
                </div>
            `;
        });
        
        inventoryItems.innerHTML = content;
        
        // Add action button event listeners
        document.querySelectorAll('.use-button').forEach(button => {
            button.addEventListener('click', (e) => {
                const itemId = e.target.closest('.inventory-item').getAttribute('data-item-id');
                // this.engine.inventoryManager.useItem(itemId);
                this.showNotification(`Used ${itemId.replace(/_/g, ' ')}`);
            });
        });
        
        document.querySelectorAll('.examine-button').forEach(button => {
            button.addEventListener('click', (e) => {
                const itemId = e.target.closest('.inventory-item').getAttribute('data-item-id');
                const item = items.find(i => i.id === itemId);
                this.showNotification(item.description);
            });
        });
    }
    
    /**
     * Show the menu screen
     */
    showMenuScreen() {
        if (!this.elements.mainContent) return;
        
        // Set content for menu screen
        this.elements.mainContent.innerHTML = `
            <div class="menu-screen">
                <h2>Game Menu</h2>
                <div class="menu-buttons">
                    <button id="save-button" class="menu-button">Save Game</button>
                    <button id="load-button" class="menu-button">Load Game</button>
                    <button id="settings-button" class="menu-button">Settings</button>
                    <button id="help-button" class="menu-button">Help</button>
                </div>
                <button id="close-menu" class="close-button">Close Menu</button>
            </div>
        `;
        
        // Add button event listeners
        document.getElementById('save-button').addEventListener('click', () => {
            this.engine.saveGame();
            this.showNotification('Game saved.');
        });
        
        document.getElementById('load-button').addEventListener('click', () => {
            const success = this.engine.loadGame();
            if (success) {
                this.showNotification('Game loaded.');
                this.toggleScreen('main');
                this.updateLocationDisplay();
            } else {
                this.showNotification('No saved game found.', 'error');
            }
        });
        
        document.getElementById('settings-button').addEventListener('click', () => {
            this.showSettingsScreen();
        });
        
        document.getElementById('help-button').addEventListener('click', () => {
            this.showHelpScreen();
        });
        
        document.getElementById('close-menu').addEventListener('click', () => {
            this.toggleScreen('main');
        });
    }
    
    /**
     * Show the settings screen
     */
    showSettingsScreen() {
        // This will be expanded with real settings options
        this.showNotification('Settings screen not implemented yet.');
    }
    
    /**
     * Show the help screen
     */
    showHelpScreen() {
        // This will be expanded with real help content
        this.showNotification('Help screen not implemented yet.');
    }
    
    /**
     * Show the character screen
     */
    showCharacterScreen() {
        if (!this.elements.mainContent) return;
        
        // Set content for character screen
        this.elements.mainContent.innerHTML = `
            <div class="character-screen">
                <h2>${this.engine.player?.name || 'Morgan Chase'}</h2>
                <div class="character-details">
                    <div class="character-traits">
                        <h3>Personality</h3>
                        <ul id="personality-traits"></ul>
                    </div>
                    <div class="character-growth">
                        <h3>Personal Growth</h3>
                        <div id="growth-meters"></div>
                    </div>
                </div>
                <button id="close-character" class="close-button">Close</button>
            </div>
        `;
        
        // Update character display
        this.updateCharacterDisplay();
        
        // Add close button handler
        document.getElementById('close-character').addEventListener('click', () => {
            this.toggleScreen('main');
        });
    }
    
    /**
     * Update the character display
     */
    updateCharacterDisplay() {
        // This will be expanded with real character data
        const traitsElement = document.getElementById('personality-traits');
        const growthElement = document.getElementById('growth-meters');
        
        if (!traitsElement || !growthElement) return;
        
        // Example traits
        const traits = this.engine.player?.personalityTraits || ['cynical', 'analytical', 'impatient'];
        
        // Example growth areas
        const growth = this.engine.player?.growthAreas || {
            empathy: 0,
            communitySpirit: 0,
            patience: 0,
            humility: 0
        };
        
        // Add traits
        let traitsHTML = '';
        traits.forEach(trait => {
            traitsHTML += `<li>${trait}</li>`;
        });
        traitsElement.innerHTML = traitsHTML;
        
        // Add growth meters
        let growthHTML = '';
        for (const [area, value] of Object.entries(growth)) {
            const formattedArea = area.replace(/([A-Z])/g, ' $1')
                .replace(/^./, str => str.toUpperCase());
                
            growthHTML += `
                <div class="growth-meter">
                    <label>${formattedArea}</label>
                    <div class="meter">
                        <div class="meter-fill" style="width: ${Math.min(100, value * 3)}%"></div>
                    </div>
                </div>
            `;
        }
        growthElement.innerHTML = growthHTML;
    }
    
    /**
     * Play a sound effect
     * @param {string} soundType - The type of sound to play
     */
    playSound(soundType) {
        // This will be expanded with real sound playback
        console.log(`Playing sound: ${soundType}`);
    }
}

// Export the UIManager class
export default UIManager;
