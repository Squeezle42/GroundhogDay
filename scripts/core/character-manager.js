/**
 * Time Loop: A Groundhog Day-Inspired Game
 * 
 * Copyright (C) 2025 Time Loop Game Development Team
 */

/**
 * CharacterManager - Handles game characters, dialogues, and relationships
 */
class CharacterManager {
    constructor(gameEngine) {
        this.engine = gameEngine;
        this.characters = {};
        this.playerRelationships = {};
        this.dialogues = {};
        
        // Track image variants for cycling through them
        this.imageVariants = {
            'mayor_proud': [
                'assets/images/characters/mayor_proud.jfif',
                'assets/images/characters/mayor_proud(2).jfif',
                'assets/images/characters/mayor_proud(3).jfif',
                'assets/images/characters/mayor_proud(4).jfif',
                'assets/images/characters/mayor_proud(5).jfif'
            ]
        };
        
        // Track which variant is currently in use
        this.currentVariant = {};
        
        // Set up interval for cycling through variants
        setInterval(() => this.cycleThroughVariants(), 5000); // Cycle every 5 seconds
    }
    
    /**
     * Helper method to encode image paths that contain spaces
     * @param {string} imagePath - The path to the image
     * @returns {string} - The encoded image path
     */
    encodeImagePath(imagePath) {
        if (!imagePath || !imagePath.includes(' ')) {
            return imagePath;
        }
        
        // Split path to preserve directory structure
        const pathParts = imagePath.split('/');
        const fileName = pathParts.pop(); // Get the filename with spaces
        const basePath = pathParts.join('/');
        
        // Encode just the filename portion
        const encodedFileName = encodeURIComponent(fileName);
        return basePath + '/' + encodedFileName;
    }
    
    async initialize() {
        try {
            // Load character data
            const charResponse = await fetch('data/characters.json');
            if (!charResponse.ok) {
                throw new Error(`Failed to load characters: ${charResponse.status}`);
            }
            this.characters = await charResponse.json();
            console.log('Characters loaded:', Object.keys(this.characters).length);
            
            // Load dialogue data
            const dialogueResponse = await fetch('data/dialogue/introduction.json');
            if (!dialogueResponse.ok) {
                throw new Error(`Failed to load dialogues: ${dialogueResponse.status}`);
            }
            this.dialogues = await dialogueResponse.json();
            console.log('Dialogues loaded:', Object.keys(this.dialogues).length);
            
            // Initialize player relationships
            for (const charId in this.characters) {
                if (charId !== 'player') {
                    this.playerRelationships[charId] = 0; // Start neutral
                }
            }
            
            return true;
        } catch (error) {
            console.error('Error initializing character manager:', error);
            return false;
        }
    }
    
    getCharacter(characterId) {
        return this.characters[characterId];
    }
    
    getPlayer() {
        return this.characters['player'];
    }
    
    getRelationship(characterId) {
        return this.playerRelationships[characterId] || 0;
    }
    
    changeRelationship(characterId, amount) {
        if (this.playerRelationships[characterId] !== undefined) {
            this.playerRelationships[characterId] += amount;
            console.log(`Relationship with ${characterId} changed by ${amount}, now ${this.playerRelationships[characterId]}`);
            return true;
        }
        return false;
    }
    
    getCharactersAtLocation(locationId, timeOfDay) {
        const npcsAtLocation = [];
        
        for (const charId in this.characters) {
            if (charId === 'player') continue;
            
            const character = this.characters[charId];
            if (character.schedule && character.schedule[timeOfDay] === locationId) {
                npcsAtLocation.push(charId);
            }
        }
        
        return npcsAtLocation;
    }
    
    startDialogue(characterId, contextKey) {
        // Find the appropriate dialogue
        let dialogueKey = null;
        
        // Try to find a specific dialogue key
        if (contextKey) {
            dialogueKey = `${characterId}_${contextKey}`;
        }
        
        // If no specific dialogue or not found, try default dialogue
        if (!dialogueKey || !this.dialogues[dialogueKey]) {
            // Find dialogues for this character
            const characterDialogues = Object.keys(this.dialogues).filter(key => key.startsWith(`${characterId}_`));
            
            if (characterDialogues.length > 0) {
                // For now, just use the first dialogue
                dialogueKey = characterDialogues[0];
            } else {
                console.error(`No dialogue found for character: ${characterId}`);
                return false;
            }
        }
        
        // Get the dialogue
        const dialogue = this.dialogues[dialogueKey];
        if (!dialogue) {
            console.error(`Dialogue not found: ${dialogueKey}`);
            return false;
        }
        
        // Show the dialogue
        this.showDialogue(dialogue);
        return true;
    }
    
    showDialogue(dialogueData) {
        // Get the dialogue container
        const dialogueContainer = document.getElementById('dialogue-container');
        if (!dialogueContainer) {
            console.error('Dialogue container not found');
            return;
        }
        
        // Show the dialogue container
        dialogueContainer.classList.remove('hidden');
        
        // Show the first dialogue entry
        this.showDialogueEntry(dialogueData.dialogue[0], dialogueData);
    }
    
    showDialogueEntry(entry, dialogueData) {
        // Get dialogue elements
        const speakerName = document.getElementById('speaker-name');
        const dialogueText = document.getElementById('dialogue-text');
        const portraitImage = document.getElementById('npc-portrait');
        const choicesContainer = document.getElementById('dialogue-choices');
        
        // Set the speaker name and text
        if (speakerName) speakerName.textContent = entry.speaker === 'player' ? 'You' : this.characters[entry.speaker]?.name || entry.speaker;
        if (dialogueText) dialogueText.textContent = entry.text;        // Set the portrait image if available
        if (portraitImage) {
            // Store the character ID as a data attribute for image cycling
            portraitImage.dataset.character = entry.speaker;
            
            let portraitPath;
            
            // Check if this entry has a specific portrait
            if (entry.portrait) {
                // Check if this is a mayor_proud reference
                if (entry.portrait.includes('mayor_proud')) {
                    portraitPath = this.getNextVariantImage(entry.portrait);
                } else {
                    portraitPath = entry.portrait;
                }
            } else {
                // Use character's default portrait
                const char = this.characters[entry.speaker];
                if (char && char.portrait) {
                    portraitPath = char.portrait;
                }
            }
            
            if (portraitPath) {
                const encodedPortrait = this.encodeImagePath(portraitPath);
                portraitImage.style.backgroundImage = `url(${encodedPortrait})`;
            } else {
                portraitImage.style.backgroundImage = 'none';
            }
        }
        
        // Clear any existing choices
        if (choicesContainer) choicesContainer.innerHTML = '';
        
        // Add choices if available
        if (entry.choices && choicesContainer) {
            entry.choices.forEach(choice => {
                const button = document.createElement('button');
                button.className = 'dialogue-choice';
                button.textContent = choice.text;
                button.addEventListener('click', () => {
                    // Go to the next part of the dialogue
                    if (choice.next) {
                        const nextEntry = dialogueData.branches[choice.next][0];
                        this.showDialogueEntry(nextEntry, dialogueData);
                    } else {
                        // End dialogue if no next
                        this.endDialogue();
                    }
                });
                choicesContainer.appendChild(button);
            });
        } else if (entry.next && dialogueData.branches[entry.next]) {
            // Auto-advance after delay if there's a next entry
            setTimeout(() => {
                const nextEntry = dialogueData.branches[entry.next][0];
                this.showDialogueEntry(nextEntry, dialogueData);
            }, 1000);
        } else {
            // End dialogue button if no choices and no next
            if (choicesContainer) {
                const button = document.createElement('button');
                button.className = 'dialogue-choice';
                button.textContent = "End conversation";
                button.addEventListener('click', () => this.endDialogue());
                choicesContainer.appendChild(button);
            }
        }
    }
      endDialogue() {
        // Hide the dialogue container
        const dialogueContainer = document.getElementById('dialogue-container');
        if (dialogueContainer) {
            dialogueContainer.classList.add('hidden');
        }
    }
    
    /**
     * Get the next image variant in the cycle
     * @param {string} baseImagePath - The base image path
     * @returns {string} - The path to the next variant image
     */
    getNextVariantImage(baseImagePath) {
        // Check if this is a base path with variants
        if (!baseImagePath) return baseImagePath;
        
        // Extract the base name without extension
        const pathParts = baseImagePath.split('/');
        const fileName = pathParts[pathParts.length - 1].split('.')[0];
        
        // Check if this is a mayor_proud image in dialogue json
        if (baseImagePath.includes('mayor_proud')) {
            const variantKey = 'mayor_proud';
            if (!this.currentVariant[variantKey]) {
                this.currentVariant[variantKey] = 0;
            }
            
            return this.imageVariants[variantKey][this.currentVariant[variantKey]];
        }
        
        // Return original if no variants found
        return baseImagePath;
    }
    
    /**
     * Cycle through image variants for all characters
     */
    cycleThroughVariants() {
        // Cycle through mayor_proud variants
        if (this.imageVariants['mayor_proud']) {
            if (!this.currentVariant['mayor_proud']) {
                this.currentVariant['mayor_proud'] = 0;
            }
            
            this.currentVariant['mayor_proud'] = 
                (this.currentVariant['mayor_proud'] + 1) % this.imageVariants['mayor_proud'].length;
                
            console.log(`Cycling to mayor image variant: ${this.currentVariant['mayor_proud']}`);
            
            // If a dialogue with the mayor is currently visible, update it
            this.updateVisibleMayorImage();
        }
    }
    
    /**
     * Update any visible mayor images on the screen
     */    updateVisibleMayorImage() {
        const portraitImage = document.getElementById('npc-portrait');
        if (portraitImage && portraitImage.dataset.character === 'mayor') {
            const currentVariant = this.currentVariant['mayor_proud'] || 0;
            const newImage = this.imageVariants['mayor_proud'][currentVariant];
            const encodedPath = this.encodeImagePath(newImage);
            portraitImage.style.backgroundImage = `url(${encodedPath})`;
        }
    }
    
    /**
     * Preload character images and variants for smooth transitions
     */
    preloadCharacterImages() {
        console.log('Preloading character images...');
        
        // Preload all mayor variants
        if (this.imageVariants['mayor_proud']) {
            this.imageVariants['mayor_proud'].forEach(imagePath => {
                const img = new Image();
                img.src = this.encodeImagePath(imagePath);
                console.log(`Preloaded image: ${imagePath}`);
            });
        }
        
        // Preload all character portraits
        for (const characterId in this.characters) {
            const character = this.characters[characterId];
            if (character.portrait) {
                const img = new Image();
                img.src = this.encodeImagePath(character.portrait);
            }
        }
    }
}

// Make the CharacterManager available globally
window.CharacterManager = CharacterManager;
