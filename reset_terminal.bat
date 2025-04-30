@echo off
echo ===== VSCode Terminal Cleanup and Restart =====
echo.

echo Killing only Node.js processes related to this GroundhogDay app...
:: Find browser-sync processes that are running in this directory
for /f "tokens=2" %%a in ('tasklist /fi "imagename eq node.exe" /v ^| findstr /i "browser-sync"') do (
    taskkill /F /PID %%a 2>nul
)
:: Kill any browser-sync command processes specifically
taskkill /F /IM browser-sync.cmd /T 2>nul

:: Alternative approach - only kill Node processes in this directory path
for /f "tokens=2" %%a in ('wmic process where "name='node.exe' and commandline like '%%GroundhogDay%%'" get processid 2^>nul ^| findstr /r "[0-9]"') do (
    taskkill /F /PID %%a 2>nul
)

echo.
echo Cleaning temporary files...
if exist ".vscode/.browse.json" del /F /Q ".vscode\.browse.json"
if exist ".vscode/.debug.json" del /F /Q ".vscode\.debug.json"

echo.
echo Starting a clean development server...
start cmd /k "npm install && npm run start-dev"

echo.
echo Terminal reset complete!
echo If VSCode terminal issues persist, please restart VSCode completely.
echo.
