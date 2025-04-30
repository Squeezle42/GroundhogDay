/**
 * Time Loop: A Groundhog Day-Inspired Game
 * 
 * Copyright (C) 2025 Time Loop Game Development Team
 */

/**
 * TimeLoopEngine - Main game engine
 */
class TimeLoopEngine {
    constructor() {
        this.currentDay = 1;
        this.playerKnowledge = new Set();
        this.timePeriods = ['morning', 'afternoon', 'evening', 'night'];
        this.currentTime = this.timePeriods[0]; // Start in morning
        
        // Game systems
        this.locationManager = new LocationManager(this);
        this.characterManager = new CharacterManager(this);
        
        // State flags
        this.loopBroken = false;
        this.initialized = false;
    }
    
    async initialize() {
        try {
            console.log('Initializing game engine...');
            
            // Initialize systems
            const locationsInitialized = await this.locationManager.initialize();
            const charactersInitialized = await this.characterManager.initialize();
            
            if (!locationsInitialized || !charactersInitialized) {
                throw new Error('Failed to initialize game systems');
            }
            
            // Start at the inn
            this.locationManager.changeLocation('inn');
            
            console.log('Game engine initialized successfully');
            this.initialized = true;
            return true;
        } catch (error) {
            console.error('Failed to initialize game engine:', error);
            return false;
        }
    }
    
    advanceTime() {
        const currentIndex = this.timePeriods.indexOf(this.currentTime);
        
        if (currentIndex === -1) {
            console.error('Invalid time period:', this.currentTime);
            return false;
        }
        
        if (currentIndex === this.timePeriods.length - 1) {
            // It's night, advance to the next day
            return this.advanceDay();
        } else {
            // Move to the next time period
            this.currentTime = this.timePeriods[currentIndex + 1];
            this.locationManager.setTime(this.currentTime);
            
            // Show notification
            const timeMessages = {
                'morning': 'The morning passes...',
                'afternoon': 'The afternoon fades away...',
                'evening': 'Evening descends upon Pinewood Hollow...',
                'night': 'Night falls over the town...'
            };
            
            this.showNotification(timeMessages[this.currentTime]);
            
            // Update UI
            this.updateTimeDisplay();
            return true;
        }
    }
    
    advanceDay() {
        // Advance the day counter
        this.currentDay++;
        
        // Reset the time to morning
        this.currentTime = this.timePeriods[0];
        
        // Show notification
        this.showNotification('You wake up to the sound of "I Got You Babe" playing on the radio... again.');
        
        // Start the new day at the inn
        this.locationManager.changeLocation('inn');
        
        // Update time display
        this.updateTimeDisplay();
        return true;
    }
    
    updateTimeDisplay() {
        // Update time display in UI
        const timeDisplay = document.getElementById('time-display');
        const dayCounter = document.getElementById('day-counter');
        
        if (timeDisplay) {
            let timeText = '';
            
            switch (this.currentTime) {
                case 'morning':
                    timeText = '7:00 AM';
                    break;
                case 'afternoon':
                    timeText = '12:30 PM';
                    break;
                case 'evening':
                    timeText = '6:15 PM';
                    break;
                case 'night':
                    timeText = '10:45 PM';
                    break;
                default:
                    timeText = 'Unknown';
            }
            
            timeDisplay.textContent = timeText;
        }
        
        if (dayCounter) {
            dayCounter.textContent = `Day ${this.currentDay}`;
        }
    }
    
    showNotification(message) {
        // Create notification container if it doesn't exist
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        // Create notification element
        const notification = document.createElement('div');
        notification.className = 'notification';
        notification.textContent = message;
        container.appendChild(notification);
        
        // Remove after delay
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => notification.remove(), 1000);
        }, 8000);
    }
    
    talkToNPC(characterId) {
        return this.characterManager.startDialogue(characterId);
    }
}

// Create global game instance
window.timeLoopGame = new TimeLoopEngine();
