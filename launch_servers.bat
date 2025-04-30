@echo off
echo ===== Development Environment Launcher =====
echo.
echo 1. Launch Web Server (HTML/CSS/JS) with Live Reload
echo 2. Run monty_python_trump_roast.py once
echo 3. Run monty_python_trump_roast.py with Live Reload
echo 4. Exit
echo.

set /p choice=Enter your choice (1-4): 

if "%choice%"=="1" (
    start cmd /k "dev_server.bat"
) else if "%choice%"=="2" (
    start cmd /k "run_monty_python_script.bat"
) else if "%choice%"=="3" (
    start cmd /k "live_reload_python_script.bat"
) else if "%choice%"=="4" (
    exit
) else (
    echo Invalid choice. Please try again.
    pause
    goto :eof
)

echo.
echo Server started in a new terminal window.
echo This window will close in 3 seconds...
timeout /t 3 > nul
