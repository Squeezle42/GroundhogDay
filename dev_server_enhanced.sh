#!/bin/bash
echo "==================================================="
echo "Time Loop Game Development Server with Live Reload"
echo "==================================================="

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "[ERROR] Node.js is required but not found."
    echo "Please install Node.js from https://nodejs.org/"
    exit 1
fi

# Check for package.json
if [ ! -f package.json ]; then
    echo "[ERROR] package.json not found."
    echo "Please make sure you're in the correct directory."
    exit 1
fi

# Install dependencies if node_modules doesn't exist
if [ ! -d node_modules ]; then
    echo "Installing project dependencies..."
    npm install
    if [ $? -ne 0 ]; then
        echo "[ERROR] Failed to install dependencies."
        exit 1
    fi
fi

# Display server information
echo "==================================================="
echo "Server configurations:"
echo "- Web Server: http://localhost:5000"
echo "- Live Reload: Enabled"
echo "- Watching: HTML, CSS, JS, Python files, and assets"
echo "==================================================="
echo ""
echo "DON'T PANIC! Press Ctrl+C to stop the server."
echo ""

# Start the development server
npm run start-dev

# If the server stops, pause so the user can read any error messages
read -p "Press Enter to continue..."
