/**
 * Time Loop: A Groundhog Day-Inspired Game
 * 
 * Copyright (C) 2025 Time Loop Game Development Team
 */

/**
 * LocationManager - Handles game locations, transitions, and location-specific events
 */
class LocationManager {
    constructor(gameEngine) {
        this.engine = gameEngine;
        this.currentLocation = null;
        this.locations = {};
        this.currentWeather = "snowing";
        this.currentTime = "morning";
        this.visitedLocations = new Set();
    }
    
    async initialize() {
        try {
            const response = await fetch('data/locations.json');
            if (!response.ok) {
                throw new Error(`Failed to load locations: ${response.status}`);
            }
            this.locations = await response.json();
            console.log('Locations loaded:', Object.keys(this.locations).length);
            return true;
        } catch (error) {
            console.error('Error loading locations:', error);
            return false;
        }
    }
    
    changeLocation(locationId) {
        if (!this.locations[locationId]) {
            console.error(`Location "${locationId}" not found`);
            return false;
        }
        
        // Check if location is available at current time
        const location = this.locations[locationId];
        if (!location.available_times.includes(this.currentTime)) {
            console.log(`Location "${location.name}" is not available at ${this.currentTime}`);
            this.showNotification(`${location.name} is closed right now. Come back later.`);
            return false;
        }
        
        // Set current location
        this.currentLocation = locationId;
        
        // Check if this is the first visit
        const isFirstVisit = !this.visitedLocations.has(locationId);
        if (isFirstVisit) {
            this.visitedLocations.add(locationId);
            if (location.firstVisitMessage) {
                this.showNotification(location.firstVisitMessage);
            }
        }
        
        // Update the UI
        this.updateLocationDisplay(location);
        
        // Generate navigation buttons
        this.generateNavigationButtons(location);
        
        // Generate interaction buttons
        this.generateInteractionButtons(location);
        
        // Check for NPCs at this location
        this.checkForNPCs(location);
        
        return true;
    }
      updateLocationDisplay(location) {
        // Update location title and description
        const titleElement = document.getElementById('location-title');
        const descElement = document.getElementById('location-description');
        
        if (titleElement) titleElement.textContent = location.name;
        if (descElement) descElement.textContent = location.description;
        
        // Update location image based on weather if applicable
        const imageElement = document.getElementById('location-image');
        if (imageElement) {
            let imagePath = location.image;
            
            // Check if there's a weather-specific image
            if (location.weather_variants && location.weather_variants[this.currentWeather]) {
                imagePath = location.weather_variants[this.currentWeather];
            }
            
            // Handle spaces in image paths by encoding the URL
            if (imagePath && imagePath.includes(' ')) {
                // Split path to preserve directory structure
                const pathParts = imagePath.split('/');
                const fileName = pathParts.pop(); // Get the filename with spaces
                const basePath = pathParts.join('/');
                
                // Encode just the filename portion
                const encodedFileName = encodeURIComponent(fileName);
                imagePath = basePath + '/' + encodedFileName;
            }
            
            imageElement.src = imagePath;
            imageElement.alt = location.name;
            
            // Log the image path to help with debugging
            console.log('Updated location image:', imagePath);
        }
    }
    
    generateNavigationButtons(location) {
        const navContainer = document.getElementById('navigation-buttons');
        if (!navContainer) return;
        
        // Clear existing buttons
        navContainer.innerHTML = '';
        
        // Create a button for each connection
        if (location.connections && location.connections.length > 0) {
            location.connections.forEach(connectionId => {
                const connectedLocation = this.locations[connectionId];
                if (connectedLocation) {
                    const button = document.createElement('button');
                    button.className = 'nav-button';
                    button.textContent = `Go to ${connectedLocation.name}`;
                    button.addEventListener('click', () => this.changeLocation(connectionId));
                    navContainer.appendChild(button);
                }
            });
        } else {
            // No connections available
            const message = document.createElement('p');
            message.textContent = 'No exits available.';
            navContainer.appendChild(message);
        }
    }
    
    generateInteractionButtons(location) {
        const interactionContainer = document.getElementById('interaction-buttons');
        if (!interactionContainer) return;
        
        // Clear existing buttons
        interactionContainer.innerHTML = '';
        
        // Create a button for each interaction
        if (location.interactions && location.interactions.length > 0) {
            location.interactions.forEach(interaction => {
                const button = document.createElement('button');
                button.className = 'interaction-button';
                button.textContent = this.getInteractionLabel(interaction);
                button.addEventListener('click', () => this.handleInteraction(interaction));
                interactionContainer.appendChild(button);
            });
        }
    }
    
    getInteractionLabel(interaction) {
        // Convert snake_case to Title Case with spaces
        const words = interaction.split('_');
        const capitalizedWords = words.map(word => word.charAt(0).toUpperCase() + word.slice(1));
        return capitalizedWords.join(' ');
    }
    
    handleInteraction(interactionId) {
        console.log(`Interaction: ${interactionId}`);
        
        switch (interactionId) {
            case 'look_out_window':
                this.showNotification('You gaze out the window. It\'s still snowing, just like yesterday... and the day before.');
                break;
            
            case 'sleep':
                this.showNotification('You lie down on the bed and close your eyes...');
                setTimeout(() => {
                    if (this.engine && typeof this.engine.advanceDay === 'function') {
                        this.engine.advanceDay();
                    } else {
                        this.showNotification('You wake up... and it\'s still the same day.');
                    }
                }, 2000);
                break;
                
            case 'check_bulletin_board':
                this.showNotification('The bulletin board shows information about the Groundhog Day Festival happening today in the town square at noon.');
                if (this.engine && this.engine.playerKnowledge) {
                    this.engine.playerKnowledge.add('groundhog_festival_details');
                }
                break;
                
            case 'sit_by_fire':
                this.showNotification('You sit by the warm, crackling fire. It feels cozy, offering a brief respite from the cold winter day.');
                break;
                
            case 'check_clock_tower':
                this.showNotification('The clock tower shows 7:00 AM. Wait... wasn\'t it showing the same time yesterday?');
                break;
                
            case 'look_at_sky':
                this.showNotification('The sky is overcast with snow clouds. According to the forecast, it should clear up tomorrow... but you\'ve heard that before.');
                break;
                
            case 'order_coffee':
                this.showNotification('You order a coffee. The waitress seems to know exactly how you like it.');
                break;
                
            case 'read_newspaper':
                this.showNotification('The headline reads: "Annual Groundhog Festival Today! Will Hollow Phil see his shadow?"');
                if (this.engine && this.engine.playerKnowledge) {
                    this.engine.playerKnowledge.add('groundhog_newspaper_article');
                }
                break;
                
            case 'browse_items':
                this.showNotification('The store shelves are stocked with various items - groceries, hardware, winter supplies, and Groundhog Festival souvenirs.');
                break;
                
            case 'purchase_item':
                this.showNotification('You don\'t really need anything at the moment.');
                break;
                
            default:
                this.showNotification('You can\'t do that right now.');
        }
    }
    
    checkForNPCs(location) {
        if (!location.npcs || location.npcs.length === 0) return;
        
        // For now, just notify player about NPCs present
        const npcList = location.npcs.map(npcId => {
            // Get NPC name if possible
            if (this.engine && this.engine.characterManager) {
                const npc = this.engine.characterManager.getCharacter(npcId);
                return npc ? npc.name : npcId;
            }
            return npcId;
        }).join(", ");
        
        this.showNotification(`You see: ${npcList}`);
    }
    
    showNotification(message) {
        // Create notification container if it doesn't exist
        let container = document.getElementById('notification-container');
        if (!container) {
            container = document.createElement('div');
            container.id = 'notification-container';
            document.body.appendChild(container);
        }
        
        // Create and display the notification
        const notification = document.createElement('div');
        notification.className = 'notification';
        notification.textContent = message;
        container.appendChild(notification);
        
        // Fade out after 8 seconds
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => {
                notification.remove();
            }, 1000);
        }, 8000);
    }
    
    setWeather(weather) {
        this.currentWeather = weather;
        // Update the location display if we're at a location
        if (this.currentLocation && this.locations[this.currentLocation]) {
            this.updateLocationDisplay(this.locations[this.currentLocation]);
        }
    }
    
    setTime(time) {
        this.currentTime = time;
        
        // Check if the current location is still available at this time
        if (this.currentLocation && this.locations[this.currentLocation]) {
            const location = this.locations[this.currentLocation];
            if (!location.available_times.includes(time)) {
                this.showNotification(`As time passes, ${location.name} is closing. You need to leave.`);
                // Force move to town_square if available, or inn as fallback
                if (this.locations['town_square'] && this.locations['town_square'].available_times.includes(time)) {
                    this.changeLocation('town_square');
                } else if (this.locations['inn'] && this.locations['inn'].available_times.includes(time)) {
                    this.changeLocation('inn');
                }
            } else {
                // Update the location display for the new time of day
                this.updateLocationDisplay(location);
            }
        }
    }
}

// Make the LocationManager available globally
window.LocationManager = LocationManager;
