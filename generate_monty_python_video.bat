@echo off
REM Python script to generate images and create a video of the Monty Python-style Trump roast scene
REM This batch file sets up the environment and runs the Python script

echo Setting up for Monty Python-style Trump roast video generation...

REM Create the necessary directories if they don't exist
if not exist "generated_images\" mkdir generated_images
if not exist "video_output\" mkdir video_output

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Python is not installed or not in PATH. Please install Python 3.8+ and try again.
    exit /b 1
)

REM Check if pip is installed
pip --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo pip is not installed or not in PATH. Please install pip and try again.
    exit /b 1
)

REM Install required Python packages if not already installed
echo Installing required Python packages...
pip install openai pillow requests tqdm ffmpeg-python >nul 2>&1

REM Run the Python script to generate images and create the video
echo Running the image generation and video creation script...
python monty_python_trump_roast.py

if %ERRORLEVEL% NEQ 0 (
    echo There was an error running the script. Please check the logs for more details.
    exit /b 1
) else (
    echo Video generation complete! The video is saved in the video_output folder.
)

echo Done!
exit /b 0
