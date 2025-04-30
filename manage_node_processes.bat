@echo off
:: =======================================
:: Node.js Process Manager for GroundhogDay
:: =======================================
echo ===== Node.js Process Manager for GroundhogDay =====

:MENU
echo.
echo 1. List all Node.js processes
echo 2. List only Node.js processes for this app
echo 3. Start development server (safe mode)
echo 4. Kill specific browser-sync process by PID
echo 5. Kill all browser-sync processes for this app
echo 6. Exit
echo.

set /p choice=Enter your choice (1-6): 

if "%choice%"=="1" goto LIST_ALL
if "%choice%"=="2" goto LIST_APP
if "%choice%"=="3" goto START_SERVER
if "%choice%"=="4" goto KILL_SPECIFIC
if "%choice%"=="5" goto KILL_ALL
if "%choice%"=="6" goto EXIT

echo Invalid choice. Please try again.
goto MENU

:LIST_ALL
echo.
echo === All Node.js processes ===
tasklist /fi "imagename eq node.exe" /v
echo.
pause
goto MENU

:LIST_APP
echo.
echo === GroundhogDay Node.js processes ===
wmic process where "name='node.exe' and (commandline like '%%GroundhogDay%%' or commandline like '%%browser-sync%%')" get processid,commandline
echo.
pause
goto MENU

:START_SERVER
echo.
echo Starting development server safely...
call npm install
npx browser-sync start --server --port 5000 --files "*.html, styles/**/*.css, scripts/**/*.js, assets/**/*" --no-notify --browser "chrome" --startPath "index.html"
goto MENU

:KILL_SPECIFIC
echo.
echo === GroundhogDay Node.js processes ===
wmic process where "name='node.exe' and (commandline like '%%GroundhogDay%%' or commandline like '%%browser-sync%%')" get processid,commandline
echo.
set /p pid=Enter the Process ID to kill: 
taskkill /F /PID %pid%
echo.
echo Process %pid% terminated.
pause
goto MENU

:KILL_ALL
echo.
echo Killing all browser-sync processes for GroundhogDay...
for /f "tokens=2" %%a in ('wmic process where "name='node.exe' and commandline like '%%GroundhogDay%%'" get processid 2^>nul ^| findstr /r "[0-9]"') do (
    echo Killing process %%a
    taskkill /F /PID %%a 2>nul
)
for /f "tokens=2" %%a in ('wmic process where "name='node.exe' and commandline like '%%browser-sync%%'" get processid 2^>nul ^| findstr /r "[0-9]"') do (
    echo Killing process %%a
    taskkill /F /PID %%a 2>nul
)
echo Done.
pause
goto MENU

:EXIT
echo Exiting...
exit /b
