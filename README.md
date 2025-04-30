# Time Loop: A Groundhog Day-Inspired Game
*Acknowledging our sources is a core value of this project - see [CREDITS.md](docs/CREDITS.md) for full attribution*

## Project Overview
Time Loop is a narrative adventure game where players experience life in a small town caught in a temporal loop. As a cynical big-city professional trapped in an endlessly repeating day, players must forge relationships with townsfolk and solve interconnected puzzles to break free from the loop.

## Key Features
- **Temporal Paradox**: Experience the same day repeatedly with knowledge retention between loops
- **Character-Driven Narrative**: Develop relationships with unique townspeople who have their own schedules and stories
- **Puzzle Progression**: Use information gathered in previous loops to unlock new pathways and solutions
- **Evolving Dialogue**: Conversations change based on player knowledge and relationship development
- **Small Town Setting**: Explore a charming winter wonderland with secrets to discover

## Development Status
This project is currently in early development with a focus on the introduction and conclusion experiences. See [ROADMAP.md](ROADMAP.md) for our planned development path and [docs/DEVELOPMENT_STRATEGY.md](docs/DEVELOPMENT_STRATEGY.md) for our phased implementation approach.

## Getting Started
See [docs/PLAYER_GUIDE.md](docs/PLAYER_GUIDE.md) for instructions on how to play the current version.

## Testing
For developers and testers, see [docs/TESTING_GUIDE.md](docs/TESTING_GUIDE.md) for instructions on how to test the current implementation.

## Deployment
For deployment instructions including Google Cloud hosting setup, see [docs/DEPLOYMENT_AND_CREDITS.md](docs/DEPLOYMENT_AND_CREDITS.md).

## Development Setup

### Prerequisites
- Node.js (v14.x or later)
- Python 3.6+ (for script generation features)
- ffmpeg (for video generation)
- Git

### Installation
1. Clone the repository:
   ```
   git clone https://github.com/your-username/groundhog-day-game.git
   cd groundhog-day-game
   ```

2. Install Node.js dependencies:
   ```
   npm install
   ```

3. Install Python dependencies (optional, for image/video generation):
   ```
   pip install -r requirements.txt
   ```

4. Run the development server:
   ```
   npm start
   ```
   
   Or use the provided batch files:
   ```
   launch_servers.bat
   ```

### Environment Variables
If you're using the image generation features, you'll need to set up your OpenAI API key:
```
# Windows
set OPENAI_API_KEY=your-api-key-here

# Linux/Mac
export OPENAI_API_KEY=your-api-key-here
```

## Git Workflow

### Branch Strategy
- `main`: Stable production-ready code
- `develop`: Integration branch for ongoing development
- `feature/*`: Feature branches for new functionality
- `bugfix/*`: Branches for fixing bugs

### Commit Guidelines
- Use descriptive commit messages that explain the change
- Start with a verb in the present tense (e.g., "Add feature" not "Added feature")
- Reference issue numbers when applicable

Example: `Add character dialogue system (#42)`

## Contribution Guidelines
1. Fork the repository
2. Create a new branch for your feature or bugfix
3. Make your changes
4. Run tests if available
5. Submit a pull request against the `develop` branch
6. Ensure your code follows the project's style guidelines

## License
This project is licensed under the GNU General Public License v3.0 (GPLv3) - a copyleft license that ensures the software and its derivatives remain free and open source. See the [LICENSE](LICENSE.md) file for details.

## Sources & Attributions

We believe in fully crediting all sources that inspire and contribute to our work:

- **"Groundhog Day" (1993)** - Columbia Pictures, directed by Harold Ramis, starring Bill Murray
- **"Rocket Man"** - Original song by Elton John with lyrics by Bernie Taupin, performed by Commander Chris Hadfield
- **Unreal Engine** - Developed by Epic Games, Inc.

For a comprehensive list of all sources, inspirations, tools, and attributions, please see our dedicated [CREDITS.md](docs/CREDITS.md) file.

## Acknowledgments

Special thanks to:

- **Bill Murray** for his iconic performance in "Groundhog Day" that defined the time loop genre
- **Commander Chris Hadfield** for his incredible rendition of "Rocket Man" featured in our end credits
- All contributors, testers, and supporters of this project
