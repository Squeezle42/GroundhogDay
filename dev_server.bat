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

:: Check if http-server is installed
call npm list -g http-server >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Installing http-server...
    call npm install -g http-server
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
