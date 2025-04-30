@echo off
echo ===================================================
echo Time Loop Game Development Server with Live Reload
echo ===================================================

:: Check if Node.js is installed
where node >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Node.js is required but not found.
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
)

:: Check for package.json and install dependencies if needed
if not exist package.json (
    echo [ERROR] package.json not found.
    echo Please make sure you're in the correct directory.
    pause
    exit /b 1
)

:: Install dependencies if node_modules doesn't exist
if not exist node_modules (
    echo Installing project dependencies...
    call npm install
    if %ERRORLEVEL% neq 0 (
        echo [ERROR] Failed to install dependencies.
        pause
        exit /b 1
    )
)

:: Display server information
echo ===================================================
echo Server configurations:
echo - Web Server: http://localhost:5000
echo - Live Reload: Enabled
echo - Watching: HTML, CSS, JS, Python files, and assets
echo ===================================================
echo.
echo DON'T PANIC! Press Ctrl+C to stop the server.
echo.

:: Start the development server
call npm run start-dev

:: If the server stops, pause so the user can read any error messages
pause
