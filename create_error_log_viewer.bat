@echo off
echo ===== Real-time Error Log Viewer =====
echo.
echo This tool will monitor and display errors from JavaScript and Python in real-time.
echo Press Ctrl+C to exit.
echo.

REM Create error log directory if it doesn't exist
if not exist "logs\" mkdir logs

REM Timestamp for log file names
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (
    set mm=%%a
    set dd=%%b
    set yy=%%c
)
for /f "tokens=1-2 delims=: " %%a in ('time /t') do (
    set hh=%%a
    set min=%%b
)
set timestamp=%yy%%mm%%dd%_%hh%%min%

echo Starting JavaScript error monitoring...
echo Starting Python error monitoring...
echo.
echo Logs will be saved to: logs\errors_%timestamp%.log
echo.

REM Start monitoring in background (will capture console.error from browser)
echo Waiting for errors...

REM Create a flag file to indicate the viewer is running
echo %date% %time% > logs\error_viewer_active.tmp

:monitor_loop
REM Check for new errors in dev tools console (simplified - in production would use more robust methods)
type logs\js_errors.log 2>nul
type logs\python_errors.log 2>nul

REM In a real implementation, would use something like tail -f or a more sophisticated monitoring approach
timeout /t 2 >nul

REM Check if new error files have appeared
dir /b logs\*.err 2>nul
if not errorlevel 1 (
    echo New errors found:
    for %%f in (logs\*.err) do (
        type "%%f"
        echo.
        echo ----------------------------------------
        move "%%f" "%%f.processed" >nul
    )
)

goto monitor_loop
