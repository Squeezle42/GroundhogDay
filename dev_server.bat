@echo off
echo Starting Time Loop Game Development Server with Live Reload...

:: Kill only browser-sync processes related to this app
echo Stopping any existing browser-sync instances for this app...
for /f "tokens=2" %%a in ('tasklist /fi "imagename eq node.exe" /v ^| findstr /i "browser-sync.*GroundhogDay"') do (
    taskkill /F /PID %%a >nul 2>nul
)
:: Kill any browser-sync command processes specifically for this path
for /f "tokens=2" %%a in ('wmic process where "name='node.exe' and commandline like '%%browser-sync%%' and commandline like '%%GroundhogDay%%'" get processid 2^>nul ^| findstr /r "[0-9]"') do (
    taskkill /F /PID %%a >nul 2>nul
)

:: Check if Node.js is installed
where node >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Node.js is required but not found.
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
)

:: Verify npm is functioning
where npm >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo npm is required but not found.
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
)

:: Check if package.json exists and is valid
if not exist "package.json" (
    echo Error: package.json not found
    echo Creating a basic package.json...
    echo { > package.json
    echo   "name": "groundhog-day-game", >> package.json
    echo   "version": "1.0.0", >> package.json
    echo   "scripts": { >> package.json
    echo     "start": "browser-sync start --server --port 5000 --files \"*.html, styles/**/*.css, scripts/**/*.js, assets/**/*\" --no-notify" >> package.json
    echo   }, >> package.json
    echo   "devDependencies": {} >> package.json
    echo } >> package.json
)

:: Install local dependencies
call npm install
:: Use locally installed browser-sync (more reliable than global)
call npm install browser-sync --save-dev
if %ERRORLEVEL% neq 0 (
    echo Error installing local browser-sync dependencies.
    echo Trying global installation...
    call npm install -g browser-sync
)

echo Starting development server with live reload on port 5000...
echo DON'T PANIC! Your game will be available at: http://localhost:5000
echo.
echo If the server fails to start, try running reset_terminal.bat
echo.

:: Try to use npx for more reliable execution
echo Using npx to run browser-sync...
call npx browser-sync start --server --port 5000 --files "*.html, styles/**/*.css, scripts/**/*.js, data/**/*.json, assets/**/*" --no-notify --browser "chrome" --startPath "index.html" --no-ui

:: Fallback if npx fails
if %ERRORLEVEL% neq 0 (
    echo Npx execution failed, trying direct browser-sync command...
    call browser-sync start --server --port 5000 --files "*.html, styles/**/*.css, scripts/**/*.js, data/**/*.json, assets/**/*" --no-notify --browser "chrome" --startPath "index.html" --no-ui
)

pause
