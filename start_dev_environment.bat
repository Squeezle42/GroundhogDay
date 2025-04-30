@echo off
echo ===================================================
echo Groundhog Day Development Environment
echo ===================================================

echo Starting development servers...

:: Start the web server (background)
start cmd /k "title Web Server with Live Reload && dev_server_enhanced.bat"

:: Wait a moment for the first server to initialize
timeout /t 3 > nul

:: Start the Python file watcher (background)
start cmd /k "title Python File Watcher && python_dev_server.bat"

echo ===================================================
echo Development Environment Started
echo - Web server: http://localhost:5000
echo - Web files: Live reload active
echo - Python files: Live reload active
echo ===================================================
echo.
echo To stop all servers, close the command prompt windows.
echo.
