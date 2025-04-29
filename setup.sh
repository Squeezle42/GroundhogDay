#!/bin/bash
# Cross-platform development setup script for Time Loop game
# Works on macOS, Linux and Windows (with Git Bash or WSL)

echo "======================================================"
echo "    Time Loop Game - Cross-Platform Development Setup    "
echo "======================================================"

# Detect OS
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    OS="Windows"
else
    OS="Unknown"
fi

echo "Detected OS: $OS"
echo ""

# Check for Node.js
if command -v node >/dev/null 2>&1; then
    NODE_VERSION=$(node -v)
    echo "✅ Node.js is installed ($NODE_VERSION)"
else
    echo "❌ Node.js not found. Please install Node.js from https://nodejs.org/"
    echo "Exiting setup..."
    exit 1
fi

# Check for npm
if command -v npm >/dev/null 2>&1; then
    NPM_VERSION=$(npm -v)
    echo "✅ npm is installed ($NPM_VERSION)"
else
    echo "❌ npm not found. Please install Node.js which includes npm."
    echo "Exiting setup..."
    exit 1
fi

# Install required npm packages
echo ""
echo "Installing required npm packages..."
npm install -g browser-sync http-server

# Create platform-specific launch scripts
echo ""
echo "Creating platform-specific launch scripts..."

# Create Windows script (batch file)
cat > launch_windows.bat << EOL
@echo off
echo Starting Time Loop Game...
start chrome --app=file:///%CD%/index.html --window-size=1280,720
echo App launched!
EOL
echo "✅ Created launch_windows.bat"

# Create macOS script
cat > launch_macos.sh << EOL
#!/bin/bash
echo "Starting Time Loop Game..."
# Try to find Chrome or Safari
if [ -d "/Applications/Google Chrome.app" ]; then
  BROWSER="/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"
  \$BROWSER --app=file://\$(pwd)/index.html --window-size=1280,720
elif [ -d "/Applications/Safari.app" ]; then
  open -a Safari index.html
else
  echo "Could not find Chrome or Safari. Please open index.html manually."
fi
echo "App launched!"
EOL
chmod +x launch_macos.sh
echo "✅ Created launch_macos.sh"

# Create Linux script
cat > launch_linux.sh << EOL
#!/bin/bash
echo "Starting Time Loop Game..."
# Try to find Chrome or Firefox
if command -v google-chrome >/dev/null 2>&1; then
  google-chrome --app=file://\$(pwd)/index.html --window-size=1280,720
elif command -v firefox >/dev/null 2>&1; then
  firefox index.html
else
  echo "Could not find Chrome or Firefox. Please open index.html manually."
fi
echo "App launched!"
EOL
chmod +x launch_linux.sh
echo "✅ Created launch_linux.sh"

# Create development server script that works on all platforms
cat > dev_server.sh << EOL
#!/bin/bash
echo "Starting Time Loop Game Development Server with Live Reload..."
browser-sync start --server --port 5000 --files "*.html, styles/*.css, scripts/**/*.js, assets/**/*" --no-notify
EOL
chmod +x dev_server.sh
echo "✅ Created dev_server.sh"

# Enhance the existing dev_server.bat for Windows
cat > dev_server.bat << EOL
@echo off
echo Starting Time Loop Game Development Server with Live Reload...

:: Check if Node.js is installed
where node >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Node.js is required but not found.
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
)

:: Check if browser-sync is installed
call npm list -g browser-sync >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Installing browser-sync...
    call npm install -g browser-sync
)

echo Starting development server with live reload on port 5000...
echo DON'T PANIC! Your game will be available at: http://localhost:5000

:: Start browser-sync with appropriate settings
call browser-sync start --server --port 5000 --files "*.html, styles/*.css, scripts/**/*.js, assets/**/*" --no-notify --browser "chrome" --startPath "index.html"

pause
EOL
echo "✅ Updated dev_server.bat"

# Create project configuration for PWA (Progressive Web App)
echo ""
echo "Setting up Progressive Web App configuration..."

# Create manifest.json for PWA
cat > manifest.json << EOL
{
  "name": "Time Loop: A Groundhog Day-inspired Game",
  "short_name": "Time Loop",
  "description": "A narrative adventure game where you're trapped in an endlessly repeating day in a small winter town.",
  "start_url": "index.html",
  "display": "standalone",
  "background_color": "#1e3a8a",
  "theme_color": "#1e3a8a",
  "icons": [
    {
      "src": "assets/images/icons/icon-72x72.png",
      "sizes": "72x72",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-96x96.png",
      "sizes": "96x96",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-128x128.png",
      "sizes": "128x128",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-144x144.png",
      "sizes": "144x144",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-152x152.png",
      "sizes": "152x152",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-192x192.png",
      "sizes": "192x192",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-384x384.png",
      "sizes": "384x384",
      "type": "image/png"
    },
    {
      "src": "assets/images/icons/icon-512x512.png",
      "sizes": "512x512",
      "type": "image/png"
    }
  ]
}
EOL
echo "✅ Created manifest.json"

# Create a simple service worker
cat > service-worker.js << EOL
// Time Loop Game Service Worker

const CACHE_NAME = 'time-loop-cache-v1';
const urlsToCache = [
  '/',
  '/index.html',
  '/styles/main.css',
  '/scripts/main.js',
  '/assets/images/ui/dontpanic.jfif'
];

self.addEventListener('install', event => {
  event.waitUntil(
    caches.open(CACHE_NAME)
      .then(cache => {
        console.log('Opened cache');
        return cache.addAll(urlsToCache);
      })
  );
});

self.addEventListener('fetch', event => {
  event.respondWith(
    caches.match(event.request)
      .then(response => {
        if (response) {
          return response;
        }
        return fetch(event.request);
      }
    )
  );
});
EOL
echo "✅ Created service-worker.js"

# Create icons directory if it doesn't exist
mkdir -p assets/images/icons
echo "✅ Created icons directory structure"

# Add link to manifest in the HTML
echo ""
echo "To complete PWA setup, you'll need to add the manifest link to your HTML <head> tag:"
echo "<link rel='manifest' href='manifest.json'>"

echo ""
echo "========================================================="
echo "    Cross-Platform Development Setup Complete!    "
echo "========================================================="
echo ""
echo "To start the development server with live reload:"
echo "  • Windows: Run dev_server.bat"
echo "  • macOS/Linux: Run ./dev_server.sh"
echo ""
echo "To launch the game as a standalone app:"
echo "  • Windows: Run launch_windows.bat"
echo "  • macOS: Run ./launch_macos.sh"
echo "  • Linux: Run ./launch_linux.sh"
echo ""
echo "For icon creation, you'll need to create the PWA icons in assets/images/icons/"
echo "Don't forget to add <link rel='manifest' href='manifest.json'> to your HTML"
echo ""
echo "DON'T PANIC! And remember... 42 is the answer."
