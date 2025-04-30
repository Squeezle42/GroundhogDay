@echo off
echo ===================================================
echo Monty Python Trump Roast Script Live Reload
echo ===================================================

:: Check if Python is installed
where python >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Python is required but not found.
    echo Please install Python from https://python.org/
    pause
    exit /b 1
)

:: Check if watchdog is installed
python -c "import watchdog" >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Installing watchdog for Python file watching...
    pip install watchdog colorama
    if %ERRORLEVEL% neq 0 (
        echo [ERROR] Failed to install watchdog.
        pause
        exit /b 1
    )
)

:: Check if python_watcher.py exists
if not exist "python_watcher.py" (
    echo [ERROR] python_watcher.py not found.
    echo Please run python_dev_server.bat first to create the watcher script.
    pause
    exit /b 1
)

:: Display information
echo ===================================================
echo Starting file watcher for monty_python_trump_roast.py
echo Any changes to the script will trigger automatic rerun
echo ===================================================
echo.

:: Start the Python file watcher specifically for the monty_python_trump_roast.py script
python python_watcher.py --script monty_python_trump_roast.py

:: If the watcher stops, pause so the user can read any error messages
pause
