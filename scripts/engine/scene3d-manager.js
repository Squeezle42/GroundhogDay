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
 * Scene3DManager - Manages 3D scene rendering and interactions
 */
class Scene3DManager {
    constructor() {
        // Scene container
        this.container = document.getElementById('scene3d-container');
        
        // Initialize if the container exists
        if (this.container) {
            this.init();
        }
        
        // Track interactive objects
        this.interactiveObjects = [];
        
        // Active object
        this.activeObject = null;
    }
    
    /**
     * Initialize the 3D scene
     */
    init() {
        // Create a canvas for the scene
        this.canvas = document.createElement('canvas');
        this.canvas.id = 'scene3d-canvas';
        this.container.appendChild(this.canvas);
        
        // Set up the canvas context
        this.ctx = this.canvas.getContext('2d');
        
        // Set canvas size to match container
        this.resizeCanvas();
        
        // Add window resize listener
        window.addEventListener('resize', () => this.resizeCanvas());
        
        // Add click listener
        this.canvas.addEventListener('click', (e) => this.handleClick(e));
        
        // Add mousemove listener for hover effects
        this.canvas.addEventListener('mousemove', (e) => this.handleMouseMove(e));
        
        // Add demo objects
        this.addDemoObjects();
        
        // Start render loop
        this.render();
    }
    
    /**
     * Resize canvas to match container
     */
    resizeCanvas() {
        if (this.container && this.canvas) {
            this.canvas.width = this.container.clientWidth;
            this.canvas.height = this.container.clientHeight;
        }
    }
    
    /**
     * Add demo interactive objects
     */
    addDemoObjects() {
        // Add a clock object
        this.addInteractiveObject({
            id: 'clock',
            x: this.canvas.width * 0.2,
            y: this.canvas.height * 0.3,
            width: 50,
            height: 50,
            color: '#8B4513',
            label: 'Alarm Clock',
            instruction: 'Click to check the time'
        });
        
        // Add a journal object
        this.addInteractiveObject({
            id: 'journal',
            x: this.canvas.width * 0.5,
            y: this.canvas.height * 0.6,
            width: 70,
            height: 40,
            color: '#654321',
            label: 'Journal',
            instruction: 'Click to read your notes'
        });
        
        // Add a window object
        this.addInteractiveObject({
            id: 'window',
            x: this.canvas.width * 0.8,
            y: this.canvas.height * 0.3,
            width: 80,
            height: 100,
            color: '#87CEEB',
            label: 'Window',
            instruction: 'Look outside at the weather'
        });
    }
    
    /**
     * Add an interactive object to the scene
     * @param {Object} objectData - Object configuration data
     */
    addInteractiveObject(objectData) {
        this.interactiveObjects.push(objectData);
        
        // Create instruction bubble for this object
        const bubble = document.createElement('div');
        bubble.className = 'instruction-bubble';
        bubble.textContent = objectData.instruction;
        bubble.id = `bubble-${objectData.id}`;
        bubble.style.left = `${objectData.x}px`;
        bubble.style.top = `${objectData.y}px`;
        this.container.appendChild(bubble);
    }
    
    /**
     * Handle click events on the canvas
     * @param {MouseEvent} e - Click event
     */
    handleClick(e) {
        // Get click coordinates
        const rect = this.canvas.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        
        // Check if an object was clicked
        const clickedObject = this.getObjectAtPosition(x, y);
        
        if (clickedObject) {
            console.log(`Clicked on ${clickedObject.label}`);
            
            // Show the instruction bubble briefly
            const bubble = document.getElementById(`bubble-${clickedObject.id}`);
            if (bubble) {
                bubble.classList.add('visible');
                
                // Hide after delay
                setTimeout(() => {
                    bubble.classList.remove('visible');
                }, 2000);
            }
            
            // Trigger action based on object
            this.handleObjectInteraction(clickedObject);
        }
    }
    
    /**
     * Handle object interaction
     * @param {Object} object - The interactive object
     */
    handleObjectInteraction(object) {
        switch (object.id) {
            case 'clock':
                alert('The alarm clock reads 6:00 AM. It\'s Groundhog Day.');
                break;
            case 'journal':
                alert('Your journal contains notes about the repeating day.');
                break;
            case 'window':
                alert('Looking outside, you see it\'s snowing heavily.');
                break;
        }
    }
    
    /**
     * Handle mouse move events for hover effects
     * @param {MouseEvent} e - Mouse event
     */
    handleMouseMove(e) {
        // Get mouse coordinates
        const rect = this.canvas.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;
        
        // Check for objects under cursor
        const hoveredObject = this.getObjectAtPosition(x, y);
        
        // Update hover states
        this.interactiveObjects.forEach(obj => {
            // Update hover state
            obj.isHovered = (obj === hoveredObject);
            
            // Update bubble visibility
            const bubble = document.getElementById(`bubble-${obj.id}`);
            if (bubble) {
                if (obj.isHovered) {
                    bubble.classList.add('visible');
                } else {
                    bubble.classList.remove('visible');
                }
            }
        });
    }
    
    /**
     * Check if there's an object at the specified position
     * @param {number} x - X coordinate
     * @param {number} y - Y coordinate
     * @returns {Object|null} The object at position or null
     */
    getObjectAtPosition(x, y) {
        return this.interactiveObjects.find(obj => 
            x >= obj.x - obj.width / 2 && 
            x <= obj.x + obj.width / 2 && 
            y >= obj.y - obj.height / 2 && 
            y <= obj.y + obj.height / 2
        );
    }
    
    /**
     * Render the scene
     */
    render() {
        // Clear the canvas
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        
        // Draw background
        this.ctx.fillStyle = '#333';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        
        // Draw the room
        this.drawRoom();
        
        // Draw all objects
        this.interactiveObjects.forEach(obj => this.drawObject(obj));
        
        // Request next frame
        requestAnimationFrame(() => this.render());
    }
    
    /**
     * Draw the room environment
     */
    drawRoom() {
        // Draw floor
        this.ctx.fillStyle = '#654321';
        this.ctx.fillRect(0, this.canvas.height * 0.7, this.canvas.width, this.canvas.height * 0.3);
        
        // Draw walls
        this.ctx.fillStyle = '#D2B48C';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height * 0.7);
    }
    
    /**
     * Draw an interactive object
     * @param {Object} obj - The object to draw
     */
    drawObject(obj) {
        // Set color (brighter if hovered)
        if (obj.isHovered) {
            this.ctx.fillStyle = this.adjustColor(obj.color, 40);
            this.ctx.strokeStyle = '#fff';
            this.ctx.lineWidth = 2;
        } else {
            this.ctx.fillStyle = obj.color;
            this.ctx.strokeStyle = 'rgba(255,255,255,0.5)';
            this.ctx.lineWidth = 1;
        }
        
        // Draw the object
        this.ctx.beginPath();
        this.ctx.rect(obj.x - obj.width / 2, obj.y - obj.height / 2, obj.width, obj.height);
        this.ctx.fill();
        this.ctx.stroke();
        
        // Draw label if hovered
        if (obj.isHovered) {
            this.ctx.fillStyle = 'white';
            this.ctx.font = '14px Arial';
            this.ctx.textAlign = 'center';
            this.ctx.fillText(obj.label, obj.x, obj.y + obj.height / 2 + 20);
        }
    }
    
    /**
     * Adjust a color to make it lighter/darker
     * @param {string} color - Color to adjust
     * @param {number} amount - Amount to lighten (positive) or darken (negative)
     * @returns {string} Adjusted color
     */
    adjustColor(color, amount) {
        // Convert hex to RGB
        let r = parseInt(color.substring(1, 3), 16);
        let g = parseInt(color.substring(3, 5), 16);
        let b = parseInt(color.substring(5, 7), 16);
        
        // Adjust RGB values
        r = Math.min(255, Math.max(0, r + amount));
        g = Math.min(255, Math.max(0, g + amount));
        b = Math.min(255, Math.max(0, b + amount));
        
        // Convert back to hex
        return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`;
    }
}

// Initialize the Scene3D manager when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.scene3dManager = new Scene3DManager();
});
