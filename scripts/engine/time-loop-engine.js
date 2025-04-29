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
 * TimeLoopEngine - The core game engine managing the time loop mechanics
 * This class handles the overall game state, time progression, and loop resets
 */
class TimeLoopEngine {
    constructor() {
        // Initialize the game state
        this.gameState = {
            currentDay: 1,          // Increments each time a loop completes
            timeOfDay: 6,           // Hours, starting at 6 AM
            minutes: 0,             // Minutes
            playerLocation: 'inn',  // Starting location
            activeNPCs: [],         // NPCs currently in the scene
            weather: 'snowing',     // Current weather condition
            inventory: [],          // Player's inventory
            knowledgeBase: {},      // Information retained between loops
            relationships: {},      // Relationship levels with NPCs
            questStatus: {}         // Status of active quests
        };

        // Configuration values
        this.config = {
            timeScale: 1,           // How quickly game time passes
            hoursPerDay: 18,        // Game day runs from 6AM to midnight (18 hours)
            minutesPerGameHour: 60, // Game minutes per hour
            realTimePerGameHour: 5  // Real-time minutes per game hour
        };

        // Time management
        this.lastTimestamp = 0;     // Last frame timestamp
        this.timeAccumulator = 0;   // Accumulated time since last game time update
        
        // Systems
        this.locationManager = null;
        this.characterManager = null;
        this.dialogueManager = null;
        this.timeManager = null;
        this.questManager = null;
        this.uiManager = null;

        // Setup event handlers
        this.setupEvents();
        
        console.log("Time Loop Engine initialized");
    }

    /**
     * Initialize all game systems
     */
    initSystems() {
        // TODO: Initialize all game system managers
        // this.locationManager = new LocationManager(this);
        // this.characterManager = new CharacterManager(this);
        // this.dialogueManager = new DialogueManager(this);
        // this.timeManager = new TimeManager(this);
        // this.questManager = new QuestManager(this);
        // this.uiManager = new UIManager(this);
        
        console.log("Game systems initialized");
    }
    
    /**
     * Setup event listeners
     */
    setupEvents() {
        document.addEventListener('locationChange', this.handleLocationChange.bind(this));
        document.addEventListener('interactionStart', this.handleInteraction.bind(this));
        document.addEventListener('dialogueEnd', this.handleDialogueEnd.bind(this));
        document.addEventListener('questUpdate', this.handleQuestUpdate.bind(this));
        
        console.log("Events initialized");
    }
    
    /**
     * Start the game loop
     */
    start() {
        this.initSystems();
        this.lastTimestamp = performance.now();
        requestAnimationFrame(this.gameLoop.bind(this));
        
        console.log("Game started");
    }
    
    /**
     * Main game loop
     * @param {number} timestamp - Current timestamp from requestAnimationFrame
     */
    gameLoop(timestamp) {
        // Calculate delta time
        const deltaTime = timestamp - this.lastTimestamp;
        this.lastTimestamp = timestamp;
        
        // Update game time
        this.updateTime(deltaTime);
        
        // Update game systems
        this.update(deltaTime);
        
        // Render the game
        this.render();
        
        // Continue the game loop
        requestAnimationFrame(this.gameLoop.bind(this));
    }
    
    /**
     * Update game time
     * @param {number} deltaTime - Time since last frame in milliseconds
     */
    updateTime(deltaTime) {
        // Convert real time to game time using timeScale
        this.timeAccumulator += deltaTime * this.config.timeScale;
        
        // Calculate how many game minutes to advance
        const realTimePerGameMinute = (this.config.realTimePerGameHour * 60000) / this.config.minutesPerGameHour;
        const minutesToAdd = Math.floor(this.timeAccumulator / realTimePerGameMinute);
        
        if (minutesToAdd > 0) {
            // Reset accumulator accounting for leftover time
            this.timeAccumulator -= minutesToAdd * realTimePerGameMinute;
            
            // Update game time
            this.gameState.minutes += minutesToAdd;
            
            // Convert excess minutes to hours
            if (this.gameState.minutes >= this.config.minutesPerGameHour) {
                const hoursToAdd = Math.floor(this.gameState.minutes / this.config.minutesPerGameHour);
                this.gameState.timeOfDay += hoursToAdd;
                this.gameState.minutes %= this.config.minutesPerGameHour;
                
                // Time-based events trigger here
                if (hoursToAdd > 0) {
                    this.onHourChanged();
                }
            }
            
            // Check for day end (midnight)
            if (this.gameState.timeOfDay >= 24) {
                this.endDay();
            }
        }
    }
    
    /**
     * Hour change handler - triggers time-based events
     */
    onHourChanged() {
        console.log(`Time is now ${this.formatGameTime()}`);
        
        // Notify other systems about time change
        document.dispatchEvent(new CustomEvent('hourChanged', {
            detail: {
                hour: this.gameState.timeOfDay,
                gameState: this.gameState
            }
        }));
        
        // Update weather occasionally
        if (Math.random() < 0.2) {
            this.updateWeather();
        }
    }
    
    /**
     * Format current game time as string (HH:MM)
     * @returns {string} Formatted time string
     */
    formatGameTime() {
        const hours = this.gameState.timeOfDay % 12 || 12; // Convert to 12-hour format
        const ampm = this.gameState.timeOfDay < 12 ? 'AM' : 'PM';
        const minutes = this.gameState.minutes.toString().padStart(2, '0');
        return `${hours}:${minutes} ${ampm}`;
    }
    
    /**
     * Update game systems
     * @param {number} deltaTime - Time since last frame in milliseconds
     */
    update(deltaTime) {
        // TODO: Update all game systems
        // this.locationManager.update(deltaTime);
        // this.characterManager.update(deltaTime);
        // this.questManager.update(deltaTime);
    }
    
    /**
     * Render the game
     */
    render() {
        // TODO: Render game state using UI Manager
        // this.uiManager.render(this.gameState);
    }
    
    /**
     * End the current day and trigger day reset
     */
    endDay() {
        console.log("Day ended, looping back to morning");
        
        // Increment the loop counter
        this.gameState.currentDay++;
        
        // Reset time to 6 AM
        this.gameState.timeOfDay = 6;
        this.gameState.minutes = 0;
        
        // Reset location to starting point
        this.gameState.playerLocation = 'inn';
        
        // Reset day-specific state but retain knowledge
        this.resetDayState();
        
        // Trigger new day event
        document.dispatchEvent(new CustomEvent('dayReset', {
            detail: {
                day: this.gameState.currentDay,
                gameState: this.gameState
            }
        }));
    }
    
    /**
     * Reset state for a new day while preserving persistent knowledge
     */
    resetDayState() {
        // Reset NPCs to starting positions
        // Reset physical world state
        // Reset quests to appropriate states based on knowledge
        // Reset character positions and schedules
        
        // TODO: Implement specific reset behavior for game systems
        // this.locationManager.resetDay();
        // this.characterManager.resetDay();
        // this.questManager.resetDay();
    }
    
    /**
     * Handle location change events
     * @param {CustomEvent} event - Location change event
     */
    handleLocationChange(event) {
        const { location } = event.detail;
        this.gameState.playerLocation = location;
        
        console.log(`Moved to ${location}`);
        
        // Update active NPCs based on location and time
        this.updateActiveNPCs();
    }
    
    /**
     * Update the list of active NPCs based on current location and time
     */
    updateActiveNPCs() {
        // TODO: Implement NPC scheduling based on time and location
        // this.gameState.activeNPCs = this.characterManager.getNPCsForLocation(
        //     this.gameState.playerLocation, 
        //     this.gameState.timeOfDay
        // );
    }
    
    /**
     * Update the weather
     */
    updateWeather() {
        const weatherTypes = ['sunny', 'snowing', 'heavy-snow', 'cloudy'];
        const currentIndex = weatherTypes.indexOf(this.gameState.weather);
        
        // Weather tends to stay the same or change gradually
        let newIndex;
        const change = Math.floor(Math.random() * 3) - 1; // -1, 0, or 1
        
        newIndex = Math.max(0, Math.min(weatherTypes.length - 1, currentIndex + change));
        this.gameState.weather = weatherTypes[newIndex];
        
        console.log(`Weather changed to ${this.gameState.weather}`);
        
        // Notify about weather change
        document.dispatchEvent(new CustomEvent('weatherChanged', {
            detail: {
                weather: this.gameState.weather
            }
        }));
    }
    
    /**
     * Handle character interactions
     * @param {CustomEvent} event - Interaction event
     */
    handleInteraction(event) {
        const { character, type } = event.detail;
        console.log(`Interacting with ${character}, type: ${type}`);
        
        // TODO: Implement interaction handling
        // this.dialogueManager.startDialogue(character, type);
    }
    
    /**
     * Handle end of dialogue
     * @param {CustomEvent} event - Dialogue end event
     */
    handleDialogueEnd(event) {
        const { character, outcome } = event.detail;
        console.log(`Dialogue with ${character} ended with outcome: ${outcome}`);
        
        // Update knowledge base with new information
        if (outcome && outcome.newKnowledge) {
            this.addKnowledge(outcome.newKnowledge);
        }
        
        // Update relationships
        if (outcome && outcome.relationshipChange) {
            this.updateRelationship(character, outcome.relationshipChange);
        }
    }
    
    /**
     * Add new knowledge to the player's knowledge base
     * @param {Object} knowledge - Knowledge to add
     */
    addKnowledge(knowledge) {
        // Knowledge persists across loops
        for (const [key, value] of Object.entries(knowledge)) {
            this.gameState.knowledgeBase[key] = value;
        }
        
        console.log("Knowledge base updated", knowledge);
        
        // Check if new knowledge affects quests
        // this.questManager.checkKnowledgeUpdates(knowledge);
    }
    
    /**
     * Update relationship with an NPC
     * @param {string} character - Character ID
     * @param {number} change - Relationship point change
     */
    updateRelationship(character, change) {
        if (!this.gameState.relationships[character]) {
            this.gameState.relationships[character] = 0;
        }
        
        this.gameState.relationships[character] += change;
        console.log(`Relationship with ${character} changed by ${change}, now ${this.gameState.relationships[character]}`);
    }
    
    /**
     * Handle quest updates
     * @param {CustomEvent} event - Quest update event
     */
    handleQuestUpdate(event) {
        const { quest, status, progress } = event.detail;
        
        if (!this.gameState.questStatus[quest]) {
            this.gameState.questStatus[quest] = { status: 'inactive', progress: 0 };
        }
        
        this.gameState.questStatus[quest].status = status;
        
        if (progress !== undefined) {
            this.gameState.questStatus[quest].progress = progress;
        }
        
        console.log(`Quest ${quest} updated: ${status}, progress: ${this.gameState.questStatus[quest].progress}`);
    }
    
    /**
     * Save game state to localStorage
     */
    saveGame() {
        const saveData = {
            gameState: this.gameState,
            timestamp: new Date().toISOString(),
            version: '0.1'
        };
        
        localStorage.setItem('timeloop_savegame', JSON.stringify(saveData));
        console.log("Game saved");
    }
    
    /**
     * Load game state from localStorage
     * @returns {boolean} Success status
     */
    loadGame() {
        const saveData = localStorage.getItem('timeloop_savegame');
        
        if (!saveData) {
            console.log("No save game found");
            return false;
        }
        
        try {
            const parsedData = JSON.parse(saveData);
            this.gameState = parsedData.gameState;
            console.log("Game loaded from", parsedData.timestamp);
            return true;
        } catch (error) {
            console.error("Failed to load save game:", error);
            return false;
        }
    }
}

// Export the engine class
export default TimeLoopEngine;
