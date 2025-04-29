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
 * LocationManager - Handles game locations, transitions, and location-specific events
 * Manages the physical spaces of Pinewood Hollow and their state across time loops
 */
class LocationManager {
    /**
     * Create a new LocationManager
     * @param {Object} gameEngine - Reference to main game engine
     */
    constructor(gameEngine) {
        this.engine = gameEngine;
        this.currentLocation = null;
        
        // Define all locations in the game
        this.locations = {
            inn: {
                name: "Hibernation Inn",
                description: "A cozy bed & breakfast where you're staying during your visit to Pinewood Hollow.",
                connections: ["town_square", "main_street"],
                timeAvailable: "all", // Location is accessible at all times
                defaultState: {
                    objects: [
                        { id: "bed", state: "made", interactable: true },
                        { id: "desk", state: "normal", interactable: true },
                        { id: "closet", state: "closed", interactable: true },
                        { id: "window", state: "closed", interactable: true }
                    ]
                },
                state: {} // Will be initialized with defaultState in resetDay
            },
            
            town_square: {
                name: "Town Square",
                description: "The heart of Pinewood Hollow, where the Winter Festival takes place.",
                connections: ["inn", "main_street", "clock_tower", "town_hall", "groundhog_grotto"],
                timeAvailable: "all",
                defaultState: {
                    objects: [
                        { id: "festival_stage", state: "setup", interactable: true },
                        { id: "benches", state: "normal", interactable: false },
                        { id: "statue", state: "normal", interactable: true },
                        { id: "decorations", state: "festival", interactable: false }
                    ]
                },
                state: {}
            },
            
            clock_tower: {
                name: "Clock Tower",
                description: "The ancient clock tower that has stood in Pinewood Hollow for over 200 years.",
                connections: ["town_square"],
                timeAvailable: [7, 21], // Available from 7 AM to 9 PM
                defaultState: {
                    objects: [
                        { id: "clock_face", state: "working", interactable: true },
                        { id: "mechanism", state: "running", interactable: true },
                        { id: "stairs", state: "normal", interactable: true },
                        { id: "observation_deck", state: "normal", interactable: true }
                    ],
                    locked: true, // Initially locked until player gets key
                    keyRequired: "clock_tower_key"
                },
                state: {}
            },
            
            main_street: {
                name: "Main Street",
                description: "The primary street of Pinewood Hollow, lined with various shops and businesses.",
                connections: ["town_square", "inn", "diner", "library", "sheriff_office"],
                timeAvailable: "all",
                defaultState: {
                    objects: [
                        { id: "storefronts", state: "normal", interactable: true },
                        { id: "streetlights", state: "off", interactable: false }
                    ]
                },
                state: {}
            },
            
            diner: {
                name: "Sophie's Diner",
                description: "A warm, inviting diner run by Sophie Chen, who seems to know everything about everyone in town.",
                connections: ["main_street"],
                timeAvailable: [6, 22], // 6 AM to 10 PM
                defaultState: {
                    objects: [
                        { id: "counter", state: "normal", interactable: true },
                        { id: "booths", state: "normal", interactable: true },
                        { id: "jukebox", state: "off", interactable: true },
                        { id: "menu_board", state: "normal", interactable: true }
                    ]
                },
                state: {}
            },
            
            library: {
                name: "Pinewood Library",
                description: "A quiet, wood-paneled library containing historical records of the town.",
                connections: ["main_street"],
                timeAvailable: [9, 19], // 9 AM to 7 PM
                defaultState: {
                    objects: [
                        { id: "bookshelves", state: "normal", interactable: true },
                        { id: "reading_area", state: "normal", interactable: true },
                        { id: "historical_section", state: "normal", interactable: true },
                        { id: "librarian_desk", state: "normal", interactable: true }
                    ]
                },
                state: {}
            },
            
            sheriff_office: {
                name: "Sheriff's Office",
                description: "The small law enforcement office run by Sheriff Kate Lawson.",
                connections: ["main_street"],
                timeAvailable: [8, 20], // 8 AM to 8 PM
                defaultState: {
                    objects: [
                        { id: "desk", state: "normal", interactable: true },
                        { id: "filing_cabinet", state: "locked", interactable: true },
                        { id: "bulletin_board", state: "normal", interactable: true },
                        { id: "cells", state: "empty", interactable: false }
                    ]
                },
                state: {}
            },
            
            town_hall: {
                name: "Town Hall",
                description: "The administrative center of Pinewood Hollow, where Mayor Winters has his office.",
                connections: ["town_square"],
                timeAvailable: [8, 17], // 8 AM to 5 PM
                defaultState: {
                    objects: [
                        { id: "reception", state: "normal", interactable: true },
                        { id: "mayors_office", state: "closed", interactable: true },
                        { id: "meeting_room", state: "normal", interactable: true },
                        { id: "archives", state: "locked", interactable: true }
                    ]
                },
                state: {}
            },
            
            groundhog_grotto: {
                name: "Groundhog Grotto",
                description: "The special area where the town's oracle groundhog lives and makes its winter prediction.",
                connections: ["town_square"],
                timeAvailable: [8, 18], // 8 AM to 6 PM
                defaultState: {
                    objects: [
                        { id: "groundhog_habitat", state: "normal", interactable: true },
                        { id: "ceremonial_area", state: "normal", interactable: true },
                        { id: "information_booth", state: "normal", interactable: true },
                        { id: "viewing_platform", state: "normal", interactable: true }
                    ]
                },
                state: {}
            },
            
            hidden_valley: {
                name: "Hidden Valley",
                description: "A secluded location connected to the town's founding, with a frozen pond.",
                connections: ["forest_path"],
                timeAvailable: "all",
                defaultState: {
                    objects: [
                        { id: "frozen_pond", state: "normal", interactable: true },
                        { id: "old_cabin", state: "locked", interactable: true },
                        { id: "strange_markings", state: "hidden", interactable: true },
                        { id: "ancient_tree", state: "normal", interactable: true }
                    ],
                    discovered: false // Player must discover this location
                },
                state: {}
            },
            
            forest_path: {
                name: "Forest Path",
                description: "A snowy trail through the woods surrounding Pinewood Hollow.",
                connections: ["town_square", "hidden_valley"],
                timeAvailable: [6, 20], // 6 AM to 8 PM (too dark after)
                defaultState: {
                    objects: [
                        { id: "trail_markers", state: "normal", interactable: true },
                        { id: "fallen_tree", state: "blocked", interactable: true },
                        { id: "animal_tracks", state: "visible", interactable: true },
                        { id: "lookout_point", state: "normal", interactable: true }
                    ],
                    hiddenValleyRevealed: false // Whether the hidden valley path is visible
                },
                state: {}
            }
        };
        
        // Initialize all location states
        this.resetDay();
    }
    
    /**
     * Reset all locations to their default state for a new day
     */
    resetDay() {
        // Clone default state for each location
        for (const [id, location] of Object.entries(this.locations)) {
            this.locations[id].state = JSON.parse(JSON.stringify(location.defaultState));
            
            // Retain knowledge-dependent states across loops
            this.applyPersistentKnowledge(id);
        }
        
        console.log("All locations reset to default state");
    }
    
    /**
     * Apply persistent knowledge to location states
     * @param {string} locationId - Location to update
     */
    applyPersistentKnowledge(locationId) {
        const knowledge = this.engine.gameState.knowledgeBase;
        const location = this.locations[locationId];
        
        // Example: If player knows about hidden valley, it stays discovered
        if (locationId === 'hidden_valley' && knowledge.hiddenValleyLocation) {
            location.state.discovered = true;
        }
        
        // Example: If player has fixed the clock tower mechanics in a previous loop
        if (locationId === 'clock_tower' && knowledge.clockTowerFixed) {
            const mechanism = location.state.objects.find(obj => obj.id === 'mechanism');
            if (mechanism) {
                mechanism.state = 'fixed';
            }
        }
        
        // Example: If player has found the key to the archives
        if (locationId === 'town_hall' && knowledge.hasArchiveKey) {
            const archives = location.state.objects.find(obj => obj.id === 'archives');
            if (archives) {
                archives.state = 'unlocked';
            }
        }
    }
    
    /**
     * Move to a new location
     * @param {string} locationId - ID of the destination location
     * @returns {boolean} Whether the move was successful
     */
    moveToLocation(locationId) {
        // Check if the location exists
        if (!this.locations[locationId]) {
            console.error(`Location ${locationId} does not exist`);
            return false;
        }
        
        // Check if location is connected to current location
        if (this.currentLocation && 
            !this.locations[this.currentLocation].connections.includes(locationId)) {
            console.error(`Cannot move directly from ${this.currentLocation} to ${locationId}`);
            return false;
        }
        
        // Check if location is available at current time
        if (!this.isLocationAvailable(locationId)) {
            console.log(`${this.locations[locationId].name} is not available at this time`);
            return false;
        }
        
        // Check for location-specific access requirements
        if (!this.checkLocationAccess(locationId)) {
            return false;
        }
        
        // Update the current location
        this.currentLocation = locationId;
        
        // Trigger location change event
        document.dispatchEvent(new CustomEvent('locationChange', {
            detail: {
                location: locationId,
                locationData: this.locations[locationId]
            }
        }));
        
        console.log(`Moved to ${this.locations[locationId].name}`);
        
        // Run location entry logic
        this.onLocationEnter(locationId);
        
        return true;
    }
    
    /**
     * Check if a location is currently available (based on time)
     * @param {string} locationId - ID of the location to check
     * @returns {boolean} Whether the location is available
     */
    isLocationAvailable(locationId) {
        const location = this.locations[locationId];
        const currentHour = this.engine.gameState.timeOfDay;
        
        // Location available at all times
        if (location.timeAvailable === 'all') {
            return true;
        }
        
        // Location available during specific hours
        if (Array.isArray(location.timeAvailable)) {
            const [openHour, closeHour] = location.timeAvailable;
            return currentHour >= openHour && currentHour < closeHour;
        }
        
        return false;
    }
    
    /**
     * Check if the player can access a location (keys, etc.)
     * @param {string} locationId - ID of the location to check
     * @returns {boolean} Whether the location is accessible
     */
    checkLocationAccess(locationId) {
        const location = this.locations[locationId];
        
        // Check if location is locked
        if (location.state.locked) {
            // Check if player has the required key
            const hasKey = this.engine.gameState.inventory.some(
                item => item.id === location.keyRequired
            );
            
            if (!hasKey) {
                console.log(`${location.name} is locked and requires a key`);
                return false;
            }
        }
        
        // Check for hidden/undiscovered locations
        if (locationId === 'hidden_valley' && !location.state.discovered) {
            if (!this.locations.forest_path.state.hiddenValleyRevealed) {
                console.log("You don't know how to get to this location yet");
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * Run logic when entering a location
     * @param {string} locationId - ID of the location
     */
    onLocationEnter(locationId) {
        const location = this.locations[locationId];
        
        // Special case: First time discovering Hidden Valley
        if (locationId === 'hidden_valley' && !location.state.discovered) {
            location.state.discovered = true;
            this.engine.addKnowledge({ hiddenValleyLocation: true });
            
            console.log("You've discovered Hidden Valley!");
            
            // Trigger discovery event
            document.dispatchEvent(new CustomEvent('locationDiscovered', {
                detail: {
                    location: locationId,
                    locationName: location.name
                }
            }));
        }
        
        // Update time-dependent objects
        this.updateLocationForTime(locationId);
    }
    
    /**
     * Update location appearance based on time of day
     * @param {string} locationId - ID of the location
     */
    updateLocationForTime(locationId) {
        const location = this.locations[locationId];
        const currentHour = this.engine.gameState.timeOfDay;
        
        // Update objects based on time
        // Examples:
        if (locationId === 'main_street') {
            // Street lights turn on at night
            const streetlights = location.state.objects.find(obj => obj.id === 'streetlights');
            if (streetlights) {
                streetlights.state = (currentHour >= 18 || currentHour < 7) ? 'on' : 'off';
            }
        }
        
        if (locationId === 'town_square') {
            // Festival activity changes throughout the day
            const stage = location.state.objects.find(obj => obj.id === 'festival_stage');
            if (stage) {
                if (currentHour < 9) stage.state = 'setup';
                else if (currentHour < 12) stage.state = 'morning_activities';
                else if (currentHour < 17) stage.state = 'main_event';
                else if (currentHour < 21) stage.state = 'evening_celebration';
                else stage.state = 'cleanup';
            }
        }
    }
    
    /**
     * Get information about the current location
     * @returns {Object} Current location data
     */
    getCurrentLocationData() {
        if (!this.currentLocation) {
            return null;
        }
        
        return {
            id: this.currentLocation,
            name: this.locations[this.currentLocation].name,
            description: this.locations[this.currentLocation].description,
            connections: this.getAvailableConnections(),
            objects: this.getInteractableObjects()
        };
    }
    
    /**
     * Get available connections from the current location
     * @returns {Array} List of available connections
     */
    getAvailableConnections() {
        if (!this.currentLocation) {
            return [];
        }
        
        return this.locations[this.currentLocation].connections.filter(
            locationId => this.isLocationAvailable(locationId)
        ).map(locationId => ({
            id: locationId,
            name: this.locations[locationId].name,
            accessible: this.checkLocationAccess(locationId)
        }));
    }
    
    /**
     * Get interactable objects in the current location
     * @returns {Array} List of interactable objects
     */
    getInteractableObjects() {
        if (!this.currentLocation) {
            return [];
        }
        
        return this.locations[this.currentLocation].state.objects.filter(
            obj => obj.interactable
        );
    }
    
    /**
     * Interact with an object in the current location
     * @param {string} objectId - ID of the object to interact with
     * @returns {Object} Result of the interaction
     */
    interactWithObject(objectId) {
        if (!this.currentLocation) {
            return { success: false, message: "No current location" };
        }
        
        const object = this.locations[this.currentLocation].state.objects.find(
            obj => obj.id === objectId && obj.interactable
        );
        
        if (!object) {
            return { success: false, message: "Object not found or not interactable" };
        }
        
        // Process interaction based on object and its state
        // This will be expanded with specific interaction logic for each object
        console.log(`Interacting with ${objectId} in ${this.currentLocation}`);
        
        // Example interactions
        if (this.currentLocation === 'clock_tower' && objectId === 'mechanism') {
            if (object.state === 'running') {
                return {
                    success: true,
                    message: "You examine the clock mechanism. It seems to be working, but something feels off about it.",
                    newState: object.state
                };
            } else if (object.state === 'broken') {
                // Check if player has knowledge to fix it
                if (this.engine.gameState.knowledgeBase.clockMechanismSolution) {
                    object.state = 'fixed';
                    this.engine.addKnowledge({ clockTowerFixed: true });
                    return {
                        success: true,
                        message: "Using your knowledge from previous loops, you fix the clock mechanism!",
                        newState: 'fixed'
                    };
                } else {
                    return {
                        success: true,
                        message: "The mechanism is broken. You don't know how to fix it yet.",
                        newState: object.state
                    };
                }
            }
        }
        
        if (this.currentLocation === 'forest_path' && objectId === 'animal_tracks') {
            if (!this.locations.forest_path.state.hiddenValleyRevealed) {
                this.locations.forest_path.state.hiddenValleyRevealed = true;
                return {
                    success: true,
                    message: "You follow the animal tracks and discover a hidden path that seems to lead somewhere special...",
                    newState: object.state,
                    discovery: "hidden_valley_path"
                };
            } else {
                return {
                    success: true,
                    message: "The animal tracks lead to the path to Hidden Valley.",
                    newState: object.state
                };
            }
        }
        
        // Generic interaction response
        return {
            success: true,
            message: `You examine the ${objectId.replace('_', ' ')}.`,
            newState: object.state
        };
    }
    
    /**
     * Update a location's state with a game event
     * @param {string} locationId - ID of the location to update
     * @param {string} eventType - Type of event
     * @param {Object} eventData - Event data
     */
    updateLocationWithEvent(locationId, eventType, eventData) {
        if (!this.locations[locationId]) {
            console.error(`Location ${locationId} does not exist`);
            return;
        }
        
        const location = this.locations[locationId];
        
        // Handle different types of events
        switch (eventType) {
            case 'npcArrived':
                // An NPC has arrived at this location
                console.log(`${eventData.npcName} arrived at ${location.name}`);
                break;
                
            case 'npcLeft':
                // An NPC has left this location
                console.log(`${eventData.npcName} left ${location.name}`);
                break;
                
            case 'weatherChange':
                // Weather has changed
                console.log(`Weather at ${location.name} changed to ${eventData.weather}`);
                break;
                
            case 'objectStateChange':
                // An object's state has changed
                const { objectId, newState } = eventData;
                const object = location.state.objects.find(obj => obj.id === objectId);
                if (object) {
                    object.state = newState;
                    console.log(`${objectId} at ${location.name} changed to ${newState}`);
                }
                break;
                
            case 'questTrigger':
                // A quest event has been triggered at this location
                console.log(`Quest event ${eventData.questEvent} triggered at ${location.name}`);
                break;
        }
        
        // If this is the current location, notify the UI to update
        if (this.currentLocation === locationId) {
            document.dispatchEvent(new CustomEvent('locationUpdated', {
                detail: {
                    location: locationId,
                    updateType: eventType,
                    updateData: eventData
                }
            }));
        }
    }
}

// Export the LocationManager class
export default LocationManager;
