@echo off
REM Script to generate a quick test clip of the Monty Python-style Trump roast scene

echo Generating test clip for Monty Python-style Trump roast video...

REM Create the necessary directories if they don't exist
if not exist "generated_images\" mkdir generated_images
if not exist "video_output\" mkdir video_output

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Python is not installed or not in PATH. Please install Python 3.8+ and try again.
    exit /b 1
)

REM Install required packages
echo Installing required Python packages...
pip install openai requests ffmpeg-python >nul 2>&1

REM Check if OpenAI API key is set
if "%OPENAI_API_KEY%"=="" (
    echo WARNING: OPENAI_API_KEY environment variable is not set.
    echo Please set it with: set OPENAI_API_KEY=your-api-key
    set /p answer="Would you like to enter your OpenAI API key now? (y/n): "
    if /i "%answer%"=="y" (
        set /p api_key="Enter your OpenAI API key: "
        set OPENAI_API_KEY=%api_key%
    ) else (
        echo You will need to set the API key in the Python script manually.
    )
)

REM Run the test clip generator
echo Running test clip generator...
python test_clip_generator.py

if %ERRORLEVEL% NEQ 0 (
    echo There was an error running the test clip generator. Please check the logs.
    exit /b 1
)

echo Test complete! If successful, your test clip is in the video_output folder.
exit /b 0
