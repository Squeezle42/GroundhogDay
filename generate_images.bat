@echo off
REM Windows batch file to run the image generation script

echo Image Generator for prompts in images.md
echo ==========================================

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: Python is not installed or not in your PATH.
    echo Please install Python 3.8 or higher and try again.
    pause
    exit /b 1
)

REM Check if images_prompts.md exists
if not exist "images_prompts.md" (
    echo Error: images_prompts.md file not found.
    echo Please create an images_prompts.md file with your prompts before running this script.
    pause
    exit /b 1
)
echo echo
REM Install required Python packages
echo Installing required packages...
pip install requests argparse >nul 2>&1

REM Create output directory if it doesn't exist
if not exist "generated_images" mkdir "generated_images"

REM Set PYTHONIOENCODING to ensure proper encoding handling
set PYTHONIOENCODING=utf-8

echo Running image generator...

echo Available styles: monty_python, cartoon, realistic, sketch
set /p STYLE="Enter image style (default: monty_python): "
if "%STYLE%"=="" set STYLE=monty_python

set /p TIMESTAMP="Create timestamped folder? (y/n, default: n): "
set TIMESTAMP_FLAG=
if /i "%TIMESTAMP%"=="y" set TIMESTAMP_FLAG=--timestamp

set /p FORCE="Force regenerate existing images? (y/n, default: n): "
set FORCE_FLAG=
if /i "%FORCE%"=="y" set FORCE_FLAG=--force

REM Run the Python script with the selected options
python generate_images_from_md.py --style %STYLE% %TIMESTAMP_FLAG% %FORCE_FLAG%

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Image generation completed successfully!
    echo Check the generated_images folder for results.
) else (
    echo.
    echo There was an error during image generation.
    echo Please check the output above for more details.
)

pause
