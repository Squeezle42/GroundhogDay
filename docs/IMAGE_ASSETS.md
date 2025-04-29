# Image Assets Documentation

## Overview

This document outlines all image assets required for the Time Loop game, including their formats, dimensions, naming conventions, and directory locations. This will serve as a reference throughout development to ensure consistency.

## General Image Requirements

### Formats
- **UI Elements**: PNG with transparency (24-bit)
- **Character Portraits**: PNG with transparency (24-bit)
- **Backgrounds**: JPG (high quality) or PNG (24-bit)
- **Location Maps**: PNG with transparency (24-bit)
- **Items**: PNG with transparency (24-bit)
- **Weather Effects**: PNG sequence with transparency (24-bit)

### Naming Convention
All image assets should follow this naming convention:
`[category]_[subcategory]_[name]_[state].[extension]`

Examples:
- `char_protagonist_alex_neutral.png`
- `loc_townsquare_day_sunny.jpg`
- `ui_button_interact_default.png`
- `item_key_rusty_inventory.png`
- **Background Images**: JPG (for locations without transparency needs) or PNG (when transparency is required)
- **Icons**: SVG (vector) or PNG (24-bit with transparency)
- **Textures**: PNG or JPG depending on transparency needs

### Naming Convention
All image files should follow this naming pattern:
- Lowercase with hyphens for spaces
- Descriptive names that indicate content and purpose
- No special characters except hyphens
- Example: `mayor-portrait-happy.png`

### Resolution Guidelines
- **UI Elements**: Design for 1920x1080, with consideration for responsive scaling
- **Character Portraits**: 512x512px minimum
- **Background Images**: 1920x1080px minimum
- **Icons**: 64x64px standard (with 128x128px high-res versions)
- **Textures**: Power of 2 dimensions (256x256, 512x512, etc.)

## Directory Structure

### assets/images/characters/
Character-related images including portraits and sprites.

#### Required Character Images

**Main Character**
- `player-portrait-neutral.png` - Default player expression
- `player-portrait-happy.png` - Happy expression
- `player-portrait-sad.png` - Sad expression
- `player-portrait-surprised.png` - Surprised expression
- `player-portrait-thoughtful.png` - Thoughtful expression
- `player-sprite-sheet.png` - Animated character sprite sheet

**NPCs**
For each NPC, the following images are needed:
- `[npc-name]-portrait-neutral.png`
- `[npc-name]-portrait-happy.png`
- `[npc-name]-portrait-sad.png`
- `[npc-name]-portrait-angry.png`
- `[npc-name]-portrait-surprised.png`
- `[npc-name]-sprite-sheet.png`

**Key NPCs Include:**
- Mayor
- Diner Owner
- Clockmaker
- Sheriff
- Librarian
- Festival Groundhog Handler
- Shopkeeper
- Bartender
- Elderly Resident
- Mysterious Stranger

## Character Assets
Located in: `assets/images/characters/`

### Main Character
- **Dimensions**: 512x1024px
- **Required States**: neutral, happy, sad, surprised, thoughtful
- **Variants**: morning, day, evening, night (lighting variations)
- **Files Needed**:
  - `char_protagonist_front_[state].png`
  - `char_protagonist_side_[state].png`
  - `char_protagonist_back_[state].png`
  - `char_protagonist_portrait_[state].png` (256x256px close-up for dialogue)

### NPCs
- **Dimensions**: 512x1024px
- **Characters Needed**:
  - Mayor (older man, distinguished)
  - Diner Owner (middle-aged woman, friendly)
  - Clockmaker (elderly, eccentric)
  - Sheriff (stern, suspicious)
  - Librarian (quiet, knowledgeable)
  - Festival Handler (stressed, enthusiastic)
  - Various Townsfolk (5-10 different designs)
- **States for Each**: neutral, talking, unique character-specific state
- **Files Example**:
  - `char_mayor_johnson_neutral.png`
  - `char_mayor_johnson_talking.png`
  - `char_mayor_johnson_ceremonial.png` (character-specific)

### assets/images/locations/
Images for game locations and backgrounds.

#### Required Location Images

**Town Areas**
- `town-square-day.jpg` - Town square during daytime
- `town-square-evening.jpg` - Town square during evening
- `town-square-night.jpg` - Town square at night
- `town-square-snowy.jpg` - Town square during snowfall

**Buildings (Exteriors)**
- `building-townhall.png` - Town hall exterior
- `building-clocktower.png` - Clock tower
- `building-diner.png` - Diner exterior
- `building-library.png` - Library exterior
- `building-sheriff.png` - Sheriff's office
- `building-shop.png` - General store
- `building-bar.png` - Local bar/pub
- `building-houses.png` - Residential houses (various)

**Buildings (Interiors)**
- `interior-townhall.jpg` - Town hall interior
- `interior-diner.jpg` - Diner interior
- `interior-library.jpg` - Library interior
- `interior-sheriff.jpg` - Sheriff's office interior
- `interior-shop.jpg` - Shop interior
- `interior-bar.jpg` - Bar interior
- `interior-player-room.jpg` - Player's hotel room

**Environmental Elements**
- `environment-trees.png` - Trees with transparency
- `environment-benches.png` - Town benches
- `environment-streetlights.png` - Street lights
- `environment-fountain.png` - Town fountain
- `environment-groundhog-statue.png` - Statue of groundhog

## Location Assets
Located in: `assets/images/locations/`

### Town Square
- **Dimensions**: 1920x1080px (background)
- **Time Variants**: morning, day, evening, night
- **Weather Variants**: clear, snowy, heavy snow
- **Files Needed**:
  - `loc_townsquare_[time]_[weather].jpg`
  - `loc_townsquare_map.png` (top-down simplified map, 512x512px)

### Other Locations
- **Main Street**: shops exterior views
- **Diner Interior**: cozy, small-town diner
- **Clock Tower**: interior and exterior
- **Library**: warm, wood-paneled interior
- **Player's Lodging**: small bed & breakfast room
- **Town Hall**: formal, administrative setting
- **Forest Path**: snowy forest trail
- **Frozen Lake**: scenic, isolated area

### assets/images/items/
Images for inventory items and interactive objects.

#### Required Item Images

**Key Items**
- `item-key-master.png` - Master key
- `item-key-library.png` - Library key
- `item-key-townhall.png` - Town hall key
- `item-clockpart.png` - Clock mechanism part

**Common Items**
- `item-coffee.png` - Coffee cup
- `item-notebook.png` - Notebook
- `item-map.png` - Town map
- `item-letter.png` - Letter
- `item-photograph.png` - Old photograph
- `item-newspaper.png` - Newspaper
- `item-flashlight.png` - Flashlight

**Collectibles**
- `collectible-coin.png` - Commemorative coin
- `collectible-postcard.png` - Town postcard
- `collectible-snowglobe.png` - Snow globe souvenir

## Items
Located in: `assets/images/items/`

- **Dimensions**: 256x256px
- **Categories**:
  - Keys & Access Items
  - Information Items (notes, photos)
  - Quest Items
  - Interactive Objects
- **Files Example**:
  - `item_key_clocktower_inventory.png`
  - `item_note_mysterious_inventory.png`
  - `item_object_groundhogstatue_scene.png`

### assets/images/ui/
User interface elements and icons.

#### Required UI Images

**Menu Elements**
- `ui-background.jpg` - Menu background
- `ui-title-logo.png` - Game title logo
- `ui-button-normal.png` - Standard button
- `ui-button-hover.png` - Button hover state
- `ui-button-pressed.png` - Button pressed state
- `ui-panel.png` - UI panel background
- `ui-frame.png` - Decorative frame for portraits

**Icons**
- `icon-inventory.png` - Inventory icon
- `icon-journal.png` - Journal icon
- `icon-settings.png` - Settings icon
- `icon-save.png` - Save game icon
- `icon-load.png` - Load game icon
- `icon-exit.png` - Exit icon
- `icon-time.png` - Time/clock icon
- `icon-energy.png` - Energy meter icon
- `icon-quest.png` - Quest marker icon
- `icon-dialogue.png` - Dialogue indicator
- `icon-interaction.png` - Interaction prompt

**Status Indicators**
- `ui-progress-bar-empty.png` - Empty progress bar
- `ui-progress-bar-fill.png` - Progress bar fill
- `ui-energy-bar-empty.png` - Empty energy bar
- `ui-energy-bar-fill.png` - Energy bar fill
- `ui-relationship-icon-neutral.png` - Neutral relationship
- `ui-relationship-icon-friendly.png` - Friendly relationship
- `ui-relationship-icon-close.png` - Close relationship

**Notifications**
- `ui-notification-background.png` - Notification background
- `ui-quest-update.png` - Quest update notification
- `ui-item-acquired.png` - Item acquisition notification

## Weather Effects
Located in: `assets/images/effects/`

- **Snow Overlay**: 1920x1080px semi-transparent
  - `effect_snow_[intensity].png` (light, medium, heavy)
- **Fog Effect**: 1920x1080px semi-transparent
  - `effect_fog_[intensity].png`
- **Day-Night Transition**: Color overlay gradients
  - `effect_daytime_[period].png` (morning, noon, evening, night)

#### Required Effect Images

**Time Loop Effects**
- `effect-time-reset.png` - Sprite sheet for time reset animation
- `effect-day-transition.png` - Day transition effect

**Weather Effects**
- `effect-snow-particle.png` - Snow particle
- `effect-rain-particle.png` - Rain particle
- `effect-fog.png` - Fog overlay

**Interaction Effects**
- `effect-highlight.png` - Object highlight
- `effect-sparkle.png` - Interaction sparkle
- `effect-thought-bubble.png` - Thought bubble for memory recall

## Image Creation Guidelines

### Style Consistency
- Maintain a cohesive art style across all assets
- Use a consistent color palette defined in the art style guide
- Characters should have consistent proportions and artistic treatment
- UI elements should share common visual language and themes

### Technical Requirements
- All transparency should be properly masked (no white halos)
- Images should be optimized for web to minimize file size
- Include @2x versions of critical UI elements for high-DPI displays
- Test all images in-game for proper scaling and rendering

### Accessibility Considerations
- Ensure sufficient contrast in UI elements
- Avoid conveying critical information through color alone
- Consider how images appear with accessibility options enabled (high contrast mode, etc.)

## Image Asset Priority List

For development purposes, create images in this priority order:

1. Core UI elements (buttons, panels, icons)
2. Main character portraits and sprites
3. Town square location (primary game hub)
4. Key NPC portraits (Mayor, Clockmaker)
5. Critical item images
6. Other locations
7. Secondary character portraits
8. Additional items
9. Visual effects

## Placeholder Strategy

During development, use these placeholder strategies:
- Colored rectangles with text labels for UI elements
- Simple silhouettes for characters
- Basic color blocks for locations
- Geometric shapes for items

## Version Control Notes
- Store all image assets in Git LFS
- Include source files (PSD, AI) in a separate "source-files" directory
- Document any significant visual changes in commit messages
