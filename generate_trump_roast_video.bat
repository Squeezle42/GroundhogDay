@echo off
REM Windows batch file to generate Monty Python Trump roast images and create a video

echo Monty Python Trump Roast - Image and Video Generator
echo ==================================================

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: Python is not installed or not in your PATH.
    echo Please install Python 3.8 or higher and try again.
    pause
    exit /b 1
)

REM Check if ffmpeg is installed
ffmpeg -version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Warning: ffmpeg is not installed or not in your PATH.
    echo The images will be generated but the video creation will be skipped.
    set SKIP_VIDEO=1
) else (
    set SKIP_VIDEO=0
)

REM Check if images_prompts.md exists
if not exist "images_prompts.md" (
    echo Error: images_prompts.md file not found.
    pause
    exit /b 1
)

REM Create required directories
if not exist "generated_images" mkdir "generated_images"
if not exist "video_output" mkdir "video_output"

REM Install required Python packages
echo Installing required packages...
pip install requests pillow >nul 2>&1

REM Step 1: Generate the images
echo.
echo Step 1: Generating Monty Python Trump roast images...
python generate_images_from_md.py --style monty_python

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error during image generation. Cannot proceed to video creation.
    pause
    exit /b 1
)

echo.
echo Images generated successfully!

REM Step 2: Create a video from the images (if ffmpeg is available)
if %SKIP_VIDEO% EQU 1 (
    echo.
    echo Skipping video creation since ffmpeg is not installed.
    echo To create a video, please install ffmpeg and add it to your PATH.
    pause
    exit /b 0
)

echo.
echo Step 2: Creating video from the generated images...

REM Create file list for ffmpeg
echo Creating file list for ffmpeg...
set FILE_LIST=video_output\files.txt
if exist "%FILE_LIST%" del "%FILE_LIST%"

REM Get all PNG files in the generated_images folder
for %%f in (generated_images\*.png) do (
    echo file '../%%f' >> "%FILE_LIST%"
    echo duration 4.0 >> "%FILE_LIST%"
)

REM Add the last image again (required by ffmpeg concat demuxer)
REM Get the last image file (no equivalent to 'tail' in Windows batch)
set LAST_IMAGE=
for %%f in (generated_images\*.png) do (
    set LAST_IMAGE=%%f
)
echo file '../%LAST_IMAGE%' >> "%FILE_LIST%"

REM Create the video
echo Creating video...
ffmpeg -y -f concat -safe 0 -i "%FILE_LIST%" -vsync vfr -pix_fmt yuv420p -c:v libx264 -r 30 -preset medium -crf 23 video_output\monty_python_roast.mp4

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error creating video.
    pause
    exit /b 1
)

echo.
echo Video created successfully!
echo The video is saved as video_output\monty_python_roast.mp4

REM Offer to play the video
set /p PLAY_VIDEO="Would you like to play the video now? (y/n): "
if /i "%PLAY_VIDEO%"=="y" (
    start video_output\monty_python_roast.mp4
)

echo.
echo Process complete!
pause
