@echo off
echo ===== Development Environment Launcher =====
echo.
echo === Game Development ===
echo 1. Launch Web Server (HTML/CSS/JS) with Live Reload
echo 2. Launch Complete Dev Environment (Web + Python + Helpers)
echo 3. Launch Game App in Browser Window
echo.
echo === Content Generation ===
echo 4. Run monty_python_trump_roast.py once
echo 5. Run monty_python_trump_roast.py with Live Reload
echo 6. Image Generation Utilities
echo.
echo === Development Helpers ===
echo 7. Monitor File Changes (Background Helper)
echo 8. Real-time Error Log Viewer
echo 9. Port Availability Checker
echo 0. Exit
echo.

set /p choice=Enter your choice (0-9): 

if "%choice%"=="0" (
    exit
) else if "%choice%"=="1" (
    start cmd /k "title Web Server with Live Reload && dev_server.bat"
) else if "%choice%"=="2" (
    start cmd /k "title Complete Dev Environment && start_dev_environment.bat"
) else if "%choice%"=="3" (
    start "" "launch_app.bat"
) else if "%choice%"=="4" (
    start cmd /k "title Monty Python Script && run_monty_python_script.bat"
) else if "%choice%"=="5" (
    start cmd /k "title Monty Python Live Reload && live_reload_python_script.bat"
) else if "%choice%"=="6" (
    start cmd /k "title Image Generation && generate_images.bat"
) else if "%choice%"=="7" (
    start cmd /k "title File Monitor && python python_watcher.py"
) else if "%choice%"=="8" (
    start cmd /k "title Error Log Viewer && create_error_log_viewer.bat"
) else if "%choice%"=="9" (
    start cmd /k "title Port Checker && check_ports.bat"
) else (
    echo Invalid choice. Please try again.
    pause
    goto :eof
)

echo.
echo Server started in a new terminal window.
echo This window will close in 3 seconds...
timeout /t 3 > nul
