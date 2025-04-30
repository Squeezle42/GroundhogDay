@echo off
echo ===================================================
echo Python Development Server with Live Reload
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

:: Create the Python file watcher script if it doesn't exist
if not exist "python_watcher.py" (
    echo Creating Python file watcher script...
    echo import sys > python_watcher.py
    echo import time >> python_watcher.py
    echo import logging >> python_watcher.py
    echo import os >> python_watcher.py
    echo import subprocess >> python_watcher.py
    echo from watchdog.observers import Observer >> python_watcher.py
    echo from watchdog.events import FileSystemEventHandler >> python_watcher.py
    echo from colorama import Fore, Style, init >> python_watcher.py
    echo. >> python_watcher.py
    echo # Initialize colorama for Windows color support >> python_watcher.py
    echo init^(^) >> python_watcher.py
    echo. >> python_watcher.py
    echo class PythonScriptHandler^(FileSystemEventHandler^): >> python_watcher.py
    echo     def __init__^(self, script_name=None^): >> python_watcher.py
    echo         self.script_name = script_name >> python_watcher.py
    echo         self.last_run_time = 0 >> python_watcher.py
    echo. >> python_watcher.py
    echo     def on_modified^(self, event^): >> python_watcher.py
    echo         if event.is_directory: >> python_watcher.py
    echo             return >> python_watcher.py
    echo. >> python_watcher.py
    echo         # Get file extension >> python_watcher.py
    echo         _, ext = os.path.splitext^(event.src_path^) >> python_watcher.py
    echo. >> python_watcher.py
    echo         # Only process .py files >> python_watcher.py
    echo         if ext.lower^(^) != '.py': >> python_watcher.py
    echo             return >> python_watcher.py
    echo. >> python_watcher.py
    echo         # If a specific script is targeted, only process that one >> python_watcher.py
    echo         if self.script_name and os.path.basename^(event.src_path^) != self.script_name: >> python_watcher.py
    echo             return >> python_watcher.py
    echo. >> python_watcher.py
    echo         # Debounce - prevent multiple runs when editor saves multiple times >> python_watcher.py
    echo         current_time = time.time^(^) >> python_watcher.py
    echo         if current_time - self.last_run_time < 2: # 2 seconds debounce >> python_watcher.py
    echo             return >> python_watcher.py
    echo         self.last_run_time = current_time >> python_watcher.py
    echo. >> python_watcher.py
    echo         print^(f"\\n{Fore.GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"^) >> python_watcher.py
    echo         print^(f"{Fore.YELLOW}🔄 File changed: {Fore.CYAN}{event.src_path}{Style.RESET_ALL}"^) >> python_watcher.py
    echo         print^(f"{Fore.GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{Style.RESET_ALL}\\n"^) >> python_watcher.py
    echo. >> python_watcher.py
    echo         # Run the Python script >> python_watcher.py
    echo         try: >> python_watcher.py
    echo             print^(f"{Fore.YELLOW}▶️ Running: {Fore.CYAN}python {event.src_path}{Style.RESET_ALL}\\n"^) >> python_watcher.py
    echo             subprocess.run^([sys.executable, event.src_path], check=True^) >> python_watcher.py
    echo             print^(f"\\n{Fore.GREEN}✅ Script completed successfully{Style.RESET_ALL}"^) >> python_watcher.py
    echo         except subprocess.CalledProcessError as e: >> python_watcher.py
    echo             print^(f"\\n{Fore.RED}❌ Script failed with error code {e.returncode}{Style.RESET_ALL}"^) >> python_watcher.py
    echo         except Exception as e: >> python_watcher.py
    echo             print^(f"\\n{Fore.RED}❌ Error: {e}{Style.RESET_ALL}"^) >> python_watcher.py
    echo. >> python_watcher.py
    echo def watch_python_files^(path='.', script_name=None^): >> python_watcher.py
    echo     event_handler = PythonScriptHandler^(script_name^) >> python_watcher.py
    echo     observer = Observer^(^) >> python_watcher.py
    echo     observer.schedule^(event_handler, path, recursive=True^) >> python_watcher.py
    echo     observer.start^(^) >> python_watcher.py
    echo. >> python_watcher.py
    echo     script_msg = f" (watching {script_name})" if script_name else "" >> python_watcher.py
    echo     print^(f"{Fore.CYAN}Python file watcher started{script_msg}.{Style.RESET_ALL}"^) >> python_watcher.py
    echo     print^(f"{Fore.CYAN}Watching for changes in .py files in {path} and its subdirectories.{Style.RESET_ALL}"^) >> python_watcher.py
    echo     print^(f"{Fore.YELLOW}Press Ctrl+C to stop.{Style.RESET_ALL}"^) >> python_watcher.py
    echo. >> python_watcher.py
    echo     try: >> python_watcher.py
    echo         while True: >> python_watcher.py
    echo             time.sleep^(1^) >> python_watcher.py
    echo     except KeyboardInterrupt: >> python_watcher.py
    echo         observer.stop^(^) >> python_watcher.py
    echo     observer.join^(^) >> python_watcher.py
    echo. >> python_watcher.py
    echo if __name__ == '__main__': >> python_watcher.py
    echo     import argparse >> python_watcher.py
    echo     parser = argparse.ArgumentParser^(description='Watch and automatically run Python files when they change.'^ >> python_watcher.py
    echo     parser.add_argument^('--script', '-s', help='Watch a specific Python script only'^ >> python_watcher.py
    echo     parser.add_argument^('--path', '-p', default='.', help='Directory to watch (default: current directory)'^ >> python_watcher.py
    echo     args = parser.parse_args^(^) >> python_watcher.py
    echo. >> python_watcher.py
    echo     watch_python_files^(args.path, args.script^ >> python_watcher.py
)

:: Display usage information
echo ===================================================
echo Python File Watcher Usage Options:
echo 1. Watch all Python files: python python_watcher.py
echo 2. Watch specific script: python python_watcher.py --script monty_python_trump_roast.py
echo ===================================================
echo.
echo Starting Python file watcher for all Python files...
echo.

:: Start the Python file watcher
python python_watcher.py

:: If the watcher stops, pause so the user can read any error messages
pause
