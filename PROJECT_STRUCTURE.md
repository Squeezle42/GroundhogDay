# Time Loop Game Project Structure

## Documentation
```
/
├── README.md                  # Project overview and getting started
├── ROADMAP.md                 # Development timeline and milestones
├── COPILOT.md                 # AI assistance guidance document
├── GROUNDHOGDAY.md            # Detailed game design document
├── GroundhogDayGameDev.md     # Initial concept document
└── docs/                      # Additional documentation
    ├── CREDITS.md             # Comprehensive source attribution (ALWAYS MAINTAIN)
    ├── SOURCE_ATTRIBUTION_GUIDELINES.md  # Guidelines for crediting sources
    ├── SOURCE_ATTRIBUTION_TEMPLATE.md    # Template for credits documentation
    ├── design/                # Design documents
    │   ├── characters/        # Character design docs
    │   ├── locations/         # Location design docs
    │   ├── mechanics/         # Game mechanics docs
    │   └── narrative/         # Story and quest docs
    ├── technical/             # Technical specifications
    │   ├── architecture/      # System architecture docs
    │   └── requirements/      # Technical requirements
    └── art/                   # Art guides and references
        ├── concept/           # Concept art
```

## Source Code
```
/
└── Source/                    # Unreal Engine project source code
    ├── TimeLoop/              # Main game module
    │   ├── TimeLoopGameMode.cpp/.h        # Game mode classes
    │   ├── Characters/        # Character classes
    │   ├── Components/        # Component classes
    │   ├── Environment/       # Environment classes
    │   ├── Systems/           # Game systems
    │   │   ├── CharacterSystem/  # NPC and character management
    │   │   ├── DialogueSystem/   # Dialogue management
    │   │   ├── QuestSystem/      # Quest management
    │   │   └── TimeSystem/       # Time loop mechanics
    │   ├── Testing/           # Test frameworks
    │   └── UI/                # UI elements
    │       ├── Widgets/       # UI widget classes
    │       └── TimeLoopHUD.cpp/.h  # HUD classes
    └── Config/                # Engine configuration
```

## Assets
```
/
└── assets/                    # Project assets
    ├── audio/                 # Audio assets
    │   ├── music/             # Music files
    │   └── sfx/               # Sound effects
    └── images/                # Visual assets
        ├── characters/        # Character images
        ├── items/             # Item images
        ├── locations/         # Location images
        └── ui/                # UI elements
```

## Data
```
/
└── data/                      # Game data files
    ├── dialogue/              # Dialogue scripts
    └── quests/                # Quest definitions
```

## Web Infrastructure
```
/
├── index.html                 # Main HTML file
├── scripts/                   # JavaScript files
│   ├── core/                  # Core functionality
│   ├── engine/                # Game engine
│   ├── ui/                    # UI management
│   └── world/                 # World management
└── styles/                    # CSS files
```

## Important Notes

### Source Attribution & Credits
All projects MUST maintain comprehensive source attribution in the following files:
1. `docs/CREDITS.md` - Detailed attribution of all sources
2. `README.md` - Brief credits section with link to CREDITS.md
3. In-game credits screen

Use the `docs/SOURCE_ATTRIBUTION_TEMPLATE.md` as a guide and follow the guidelines in `docs/SOURCE_ATTRIBUTION_GUIDELINES.md`. Proper source attribution is a non-negotiable requirement for all projects.

### Documentation First
For each major feature:
1. Document the design before implementation
2. Update documentation as implementation progresses
3. Ensure final documentation matches the implemented feature

### Testing
All features should include:
1. Unit tests where applicable
2. Integration tests for system interactions
3. Documentation for manual testing procedures
