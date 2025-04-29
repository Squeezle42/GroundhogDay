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
 * CharacterManager - Manages NPCs, their schedules, behaviors, and memories
 * Handles both the player character and all non-player characters in the game
 */
class CharacterManager {
    /**
     * Create a new CharacterManager
     * @param {Object} gameEngine - Reference to main game engine
     */
    constructor(gameEngine) {
        this.engine = gameEngine;
        
        // Player character data
        this.player = {
            name: "Morgan Chase",
            profession: "Meteorologist",
            personalityTraits: ["cynical", "analytical", "impatient"],
            growthAreas: {
                empathy: 0,        // Grows as player helps others
                communitySpirit: 0, // Grows as player participates in town events
                patience: 0,        // Grows as player completes multi-day tasks
                humility: 0         // Grows as player admits mistakes or limitations
            }
        };
        
        // NPC definitions
        this.characters = {
            mayor_winters: {
                id: "mayor_winters",
                name: "Mayor Richard Winters",
                title: "Town Mayor",
                description: "A pompous man in his 60s who takes great pride in Pinewood Hollow's traditions.",
                initialLocation: "town_hall",
                personality: ["proud", "traditional", "worried"],
                relationships: {
                    clockmaker_eliza: "strained", // They disagree about town development
                    sheriff_kate: "professional",
                    player: "neutral"
                },
                secrets: [
                    "Facing town financial crisis",
                    "Considering selling historic land to developers",
                    "Doesn't actually believe in the groundhog prediction"
                ],
                // Character's daily schedule
                schedule: [
                    { time: 6, location: "inn", activity: "breakfast" },
                    { time: 8, location: "town_hall", activity: "work" },
                    { time: 12, location: "diner", activity: "lunch" },
                    { time: 13, location: "town_hall", activity: "work" },
                    { time: 17, location: "town_square", activity: "inspection" },
                    { time: 18, location: "diner", activity: "dinner" },
                    { time: 20, location: "inn", activity: "rest" }
                ],
                dialogueTrees: "mayor_winters_dialogue", // Reference to dialogue data
                questRelated: ["save_festival", "town_finances"],
                state: {} // Current state (set in resetDay)
            },
            
            clockmaker_eliza: {
                id: "clockmaker_eliza",
                name: "Eliza Hammond",
                title: "Town Clockmaker",
                description: "An eccentric elderly woman who maintains the ancient clock tower with mysterious knowledge.",
                initialLocation: "clock_tower",
                personality: ["eccentric", "knowledgeable", "secretive"],
                relationships: {
                    mayor_winters: "distrustful",
                    librarian_marcus: "friendly", // They both value town history
                    player: "neutral"
                },
                secrets: [
                    "Descended from town founder",
                    "Knows true purpose of clock tower",
                    "Can sense the time anomaly"
                ],
                schedule: [
                    { time: 7, location: "clock_tower", activity: "maintenance" },
                    { time: 12, location: "diner", activity: "lunch" },
                    { time: 13, location: "clock_tower", activity: "work" },
                    { time: 17, location: "library", activity: "research" },
                    { time: 19, location: "diner", activity: "dinner" },
                    { time: 21, location: "inn", activity: "rest" }
                ],
                dialogueTrees: "clockmaker_eliza_dialogue",
                questRelated: ["fix_clock_tower", "founders_secret"],
                state: {}
            },
            
            diner_sophie: {
                id: "diner_sophie",
                name: "Sophie Chen",
                title: "Diner Owner",
                description: "A warm, perceptive woman who runs the town diner and knows everyone's business.",
                initialLocation: "diner",
                personality: ["observant", "kind", "gossipy"],
                relationships: {
                    sheriff_kate: "friendly",
                    groundhog_handler: "maternal", // She looks after him
                    player: "neutral"
                },
                secrets: [
                    "Lost her husband in accident five years ago",
                    "Considering selling the diner",
                    "Collects stories for a novel she's writing"
                ],
                schedule: [
                    { time: 5, location: "diner", activity: "prep" },
                    { time: 6, location: "diner", activity: "work" },
                    { time: 15, location: "town_square", activity: "break" },
                    { time: 16, location: "diner", activity: "work" },
                    { time: 22, location: "inn", activity: "rest" }
                ],
                dialogueTrees: "diner_sophie_dialogue",
                questRelated: ["town_gossip", "reconnect_relationships"],
                state: {}
            },
            
            sheriff_kate: {
                id: "sheriff_kate",
                name: "Kate Lawson",
                title: "Town Sheriff",
                description: "A no-nonsense sheriff with a suspicious attitude toward outsiders.",
                initialLocation: "sheriff_office",
                personality: ["serious", "observant", "protective"],
                relationships: {
                    mayor_winters: "respectful",
                    diner_sophie: "friendly",
                    player: "suspicious"
                },
                secrets: [
                    "Investigating disappearances related to clock tower",
                    "Has a personal connection to a cold case",
                    "Doubtful about town legends but has seen strange things"
                ],
                schedule: [
                    { time: 8, location: "sheriff_office", activity: "work" },
                    { time: 12, location: "diner", activity: "lunch" },
                    { time: 13, location: "town_square", activity: "patrol" },
                    { time: 15, location: "sheriff_office", activity: "work" },
                    { time: 18, location: "diner", activity: "dinner" },
                    { time: 19, location: "town_hall", activity: "meeting" },
                    { time: 21, location: "sheriff_office", activity: "paperwork" }
                ],
                dialogueTrees: "sheriff_kate_dialogue",
                questRelated: ["cold_case", "strange_incidents"],
                state: {}
            },
            
            librarian_marcus: {
                id: "librarian_marcus",
                name: "Marcus Wells",
                title: "Town Librarian",
                description: "A quiet, knowledgeable man who manages the town's historical records and library.",
                initialLocation: "library",
                personality: ["bookish", "thoughtful", "detail-oriented"],
                relationships: {
                    clockmaker_eliza: "respectful",
                    mayor_winters: "formal",
                    player: "neutral"
                },
                secrets: [
                    "Cataloguing supernatural occurrences in town",
                    "Found ancient documents about time phenomena",
                    "Has a theory about the groundhog legend's origin"
                ],
                schedule: [
                    { time: 9, location: "library", activity: "work" },
                    { time: 12, location: "diner", activity: "lunch" },
                    { time: 13, location: "library", activity: "work" },
                    { time: 17, location: "clock_tower", activity: "research" },
                    { time: 19, location: "diner", activity: "dinner" },
                    { time: 21, location: "inn", activity: "rest" }
                ],
                dialogueTrees: "librarian_marcus_dialogue",
                questRelated: ["town_history", "celestial_research"],
                state: {}
            },
            
            groundhog_handler: {
                id: "groundhog_handler",
                name: "Harold Jenkins",
                title: "Groundhog Handler",
                description: "An anxious man responsible for the ceremonial groundhog and its prediction.",
                initialLocation: "groundhog_grotto",
                personality: ["anxious", "superstitious", "dedicated"],
                relationships: {
                    mayor_winters: "intimidated",
                    diner_sophie: "appreciative",
                    player: "neutral"
                },
                secrets: [
                    "Groundhog escaped once during ceremony",
                    "Believes groundhog actually communicates with him",
                    "Festival brings back painful childhood memories"
                ],
                schedule: [
                    { time: 7, location: "groundhog_grotto", activity: "care" },
                    { time: 11, location: "town_square", activity: "preparation" },
                    { time: 13, location: "diner", activity: "lunch" },
                    { time: 14, location: "groundhog_grotto", activity: "work" },
                    { time: 18, location: "diner", activity: "dinner" },
                    { time: 20, location: "inn", activity: "rest" }
                ],
                dialogueTrees: "groundhog_handler_dialogue",
                questRelated: ["groundhog_meaning", "festival_preparation"],
                state: {}
            }
        };
        
        // Initialize all NPCs
        this.resetDay();
        
        // Set up event listeners
        document.addEventListener('hourChanged', this.onHourChanged.bind(this));
    }
    
    /**
     * Reset all characters to their default state for a new day
     */
    resetDay() {
        // Reset each NPC to their initial state
        for (const [id, character] of Object.entries(this.characters)) {
            this.characters[id].state = {
                currentLocation: character.initialLocation,
                currentActivity: this.getScheduledActivity(character, 6), // Start at 6 AM
                mood: "neutral",
                interactedToday: false,
                revealsAvailable: true,
                specialEventTriggered: false
            };
        }
        
        console.log("All characters reset to default state");
    }
    
    /**
     * Get a character's scheduled activity at a given hour
     * @param {Object} character - The character object
     * @param {number} hour - The hour (0-23)
     * @returns {string} The activity description
     */
    getScheduledActivity(character, hour) {
        // Find the most recent schedule entry for this hour
        for (let i = character.schedule.length - 1; i >= 0; i--) {
            if (character.schedule[i].time <= hour) {
                return character.schedule[i].activity;
            }
        }
        
        // Default if no schedule found
        return "idle";
    }
    
    /**
     * Get a character's scheduled location at a given hour
     * @param {Object} character - The character object
     * @param {number} hour - The hour (0-23)
     * @returns {string} The location ID
     */
    getScheduledLocation(character, hour) {
        // Find the most recent schedule entry for this hour
        for (let i = character.schedule.length - 1; i >= 0; i--) {
            if (character.schedule[i].time <= hour) {
                return character.schedule[i].location;
            }
        }
        
        // Default to initial location if no schedule found
        return character.initialLocation;
    }
    
    /**
     * Handle hour change event to update NPC locations and activities
     * @param {CustomEvent} event - The hour changed event
     */
    onHourChanged(event) {
        const { hour } = event.detail;
        
        // Update each NPC based on their schedule
        for (const [id, character] of Object.entries(this.characters)) {
            this.updateCharacterForTime(id, hour);
        }
    }
    
    /**
     * Update a character's state based on the current time
     * @param {string} characterId - The character's ID
     * @param {number} hour - The current hour
     */
    updateCharacterForTime(characterId, hour) {
        const character = this.characters[characterId];
        if (!character) return;
        
        // Get the scheduled location and activity for this hour
        const newLocation = this.getScheduledLocation(character, hour);
        const newActivity = this.getScheduledActivity(character, hour);
        
        const oldLocation = character.state.currentLocation;
        
        // Update the character's state
        character.state.currentLocation = newLocation;
        character.state.currentActivity = newActivity;
        
        // Handle location change events if the character moved
        if (oldLocation !== newLocation) {
            console.log(`${character.name} moved from ${oldLocation} to ${newLocation} (${newActivity})`);
            
            // Notify that character left old location
            document.dispatchEvent(new CustomEvent('characterMoved', {
                detail: {
                    character: characterId,
                    fromLocation: oldLocation,
                    toLocation: newLocation,
                    activity: newActivity
                }
            }));
            
            // Update location managers
            this.engine.locationManager?.updateLocationWithEvent(oldLocation, 'npcLeft', {
                npcId: characterId,
                npcName: character.name
            });
            
            this.engine.locationManager?.updateLocationWithEvent(newLocation, 'npcArrived', {
                npcId: characterId,
                npcName: character.name
            });
        }
        
        // Check for special time-based events for this character
        this.checkCharacterTimeEvents(characterId, hour);
    }
    
    /**
     * Check for special time-based events for a character
     * @param {string} characterId - The character's ID
     * @param {number} hour - The current hour
     */
    checkCharacterTimeEvents(characterId, hour) {
        // This will be expanded with specific character events
        const character = this.characters[characterId];
        
        // Example: Mayor's special festival announcement
        if (characterId === 'mayor_winters' && hour === 12 && character.state.currentLocation === 'town_square') {
            if (!character.state.specialEventTriggered) {
                character.state.specialEventTriggered = true;
                
                document.dispatchEvent(new CustomEvent('specialEvent', {
                    detail: {
                        type: 'mayorAnnouncement',
                        characterId: characterId,
                        location: character.state.currentLocation,
                        description: "Mayor Winters makes an announcement about the Winter Festival."
                    }
                }));
            }
        }
        
        // Example: Clockmaker's observation of the tower at sunset
        if (characterId === 'clockmaker_eliza' && hour === 17 && character.state.currentLocation === 'clock_tower') {
            if (!character.state.specialEventTriggered) {
                character.state.specialEventTriggered = true;
                
                document.dispatchEvent(new CustomEvent('specialEvent', {
                    detail: {
                        type: 'clockmakerObservation',
                        characterId: characterId,
                        location: character.state.currentLocation,
                        description: "Eliza carefully observes the clock tower as the sun sets."
                    }
                }));
            }
        }
    }
    
    /**
     * Get NPCs currently at a specific location
     * @param {string} locationId - The location ID
     * @returns {Array} Array of character objects at the location
     */
    getNPCsAtLocation(locationId) {
        return Object.values(this.characters).filter(
            character => character.state.currentLocation === locationId
        );
    }
    
    /**
     * Get a character by their ID
     * @param {string} characterId - The character's ID
     * @returns {Object} The character object
     */
    getCharacter(characterId) {
        return this.characters[characterId] || null;
    }
    
    /**
     * Get details about a character for display
     * @param {string} characterId - The character's ID
     * @returns {Object} Formatted character details
     */
    getCharacterDetails(characterId) {
        const character = this.getCharacter(characterId);
        if (!character) return null;
        
        // Return only the information the player should know
        // This will change based on relationship and knowledge
        const knowledgeLevel = this.getKnowledgeLevel(characterId);
        
        const details = {
            id: character.id,
            name: character.name,
            title: character.title,
            description: character.description,
            currentActivity: character.state.currentActivity,
            mood: character.state.mood
        };
        
        // Add more details based on knowledge level
        if (knowledgeLevel >= 1) {
            details.personality = character.personality;
        }
        
        if (knowledgeLevel >= 2) {
            details.schedule = character.schedule.map(entry => ({
                time: entry.time,
                location: entry.location,
                activity: entry.activity
            }));
        }
        
        if (knowledgeLevel >= 3) {
            details.relationships = character.relationships;
        }
        
        return details;
    }
    
    /**
     * Get the player's knowledge level about a character
     * @param {string} characterId - The character's ID
     * @returns {number} Knowledge level (0-3)
     */
    getKnowledgeLevel(characterId) {
        const knowledge = this.engine.gameState.knowledgeBase;
        
        // Check specific knowledge keys for this character
        const characterKey = `knowledge_${characterId}`;
        return knowledge[characterKey] || 0;
    }
    
    /**
     * Start an interaction with a character
     * @param {string} characterId - The character's ID
     * @returns {Object} Interaction data
     */
    interactWithCharacter(characterId) {
        const character = this.getCharacter(characterId);
        if (!character) {
            return { success: false, message: "Character not found" };
        }
        
        // Check if character is in the same location as player
        if (character.state.currentLocation !== this.engine.gameState.playerLocation) {
            return { success: false, message: `${character.name} is not here.` };
        }
        
        console.log(`Starting interaction with ${character.name}`);
        
        // Mark the character as interacted with today
        character.state.interactedToday = true;
        
        // Return interaction data
        return {
            success: true,
            character: characterId,
            characterName: character.name,
            characterActivity: character.state.currentActivity,
            characterMood: character.state.mood,
            dialogueTree: character.dialogueTrees
        };
    }
    
    /**
     * Update a character's mood based on interaction outcome
     * @param {string} characterId - The character's ID
     * @param {string} mood - The new mood
     * @param {number} relationshipChange - Change to relationship value
     */
    updateCharacterMood(characterId, mood, relationshipChange = 0) {
        const character = this.getCharacter(characterId);
        if (!character) return;
        
        character.state.mood = mood;
        
        // Update relationship if not already tracked
        if (!this.engine.gameState.relationships[characterId]) {
            this.engine.gameState.relationships[characterId] = 0;
        }
        
        // Apply relationship change
        this.engine.gameState.relationships[characterId] += relationshipChange;
        
        console.log(`${character.name}'s mood changed to ${mood}, relationship ${relationshipChange >= 0 ? '+' : ''}${relationshipChange}`);
    }
    
    /**
     * Reveal a secret from a character based on relationship level
     * @param {string} characterId - The character's ID
     * @returns {Object} The revealed secret or null
     */
    revealCharacterSecret(characterId) {
        const character = this.getCharacter(characterId);
        if (!character || !character.state.revealsAvailable) return null;
        
        const relationship = this.engine.gameState.relationships[characterId] || 0;
        
        // Secrets are revealed based on relationship level
        if (relationship < 10) {
            // Not enough relationship to reveal secrets
            return null;
        }
        
        // Determine which secret to reveal based on relationship level
        const secretIndex = Math.min(
            Math.floor(relationship / 10) - 1,
            character.secrets.length - 1
        );
        
        const secret = character.secrets[secretIndex];
        
        // Mark that a secret was revealed today
        character.state.revealsAvailable = false;
        
        console.log(`${character.name} revealed a secret: ${secret}`);
        
        // Add to knowledge base
        const knowledgeKey = `secret_${characterId}_${secretIndex}`;
        this.engine.addKnowledge({ [knowledgeKey]: secret });
        
        return { secret, index: secretIndex };
    }
    
    /**
     * Update player character growth based on actions
     * @param {string} area - Growth area (empathy, communitySpirit, patience, humility)
     * @param {number} amount - Amount to increase
     */
    updatePlayerGrowth(area, amount) {
        if (this.player.growthAreas.hasOwnProperty(area)) {
            this.player.growthAreas[area] += amount;
            console.log(`Player ${area} increased by ${amount}, now at ${this.player.growthAreas[area]}`);
            
            // Check for character development milestones
            this.checkPlayerGrowthMilestones();
        }
    }
    
    /**
     * Check if player has reached growth milestones
     */
    checkPlayerGrowthMilestones() {
        const { empathy, communitySpirit, patience, humility } = this.player.growthAreas;
        
        // Check for personality trait changes
        if (empathy >= 20 && this.player.personalityTraits.includes('cynical')) {
            // Replace cynical with empathetic
            this.player.personalityTraits = this.player.personalityTraits.filter(t => t !== 'cynical');
            this.player.personalityTraits.push('empathetic');
            
            document.dispatchEvent(new CustomEvent('playerGrowth', {
                detail: {
                    type: 'traitChange',
                    old: 'cynical',
                    new: 'empathetic',
                    message: "You find yourself genuinely caring about the problems of others."
                }
            }));
        }
        
        if (communitySpirit >= 25 && patience >= 15) {
            // Player has developed enough to progress the main story
            document.dispatchEvent(new CustomEvent('playerGrowth', {
                detail: {
                    type: 'majorDevelopment',
                    development: 'community_connection',
                    message: "You feel a genuine connection to Pinewood Hollow and its residents."
                }
            }));
            
            // Add to knowledge base
            this.engine.addKnowledge({ playerGrowth_communityConnection: true });
        }
    }
}

// Export the CharacterManager class
export default CharacterManager;
