@echo off
REM Script to generate images based on prompts in images.md

echo Image Generator for Monty Python-style scenes
echo ============================================

REM Create output directory if it doesn't exist
if not exist "generated_images\" mkdir generated_images

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Python is not installed or not in PATH. Please install Python 3.8+ and try again.
    exit /b 1
)

REM Install required packages
echo Installing required Python packages...
pip install requests >nul 2>&1

REM Present menu of options
echo.
echo Please select an image generation option:
echo 1. Generate all images (Monty Python style)
echo 2. Generate all images (Realistic style)
echo 3. Generate all images (Cartoon style)
echo 4. Generate all images (Sketch style)
echo 5. Force regenerate all images (Monty Python style)
echo 6. Exit
echo.

set /p choice="Enter your choice (1-6): "

if "%choice%"=="1" (
    python generate_images_from_md.py --style monty_python
) else if "%choice%"=="2" (
    python generate_images_from_md.py --style realistic
) else if "%choice%"=="3" (
    python generate_images_from_md.py --style cartoon
) else if "%choice%"=="4" (
    python generate_images_from_md.py --style sketch
) else if "%choice%"=="5" (
    python generate_images_from_md.py --style monty_python --force
) else if "%choice%"=="6" (
    echo Exiting...
    exit /b 0
) else (
    echo Invalid choice. Please run the script again.
    exit /b 1
)

echo.
echo Image generation process complete!
echo.
echo You can find your generated images in the "generated_images" folder.
echo Use these images with ffmpeg to create a video as described in images.md.

REM Ask if user wants to create a video
echo.
echo Would you like to create a video from the generated images?
set /p create_video="Enter y/n: "

if /i "%create_video%"=="y" (
    echo Creating video from generated images...
    
    REM Check if ffmpeg is installed
    ffmpeg -version >nul 2>&1
    if %ERRORLEVEL% NEQ 0 (
        echo ffmpeg is not installed or not in PATH. 
        echo Please install ffmpeg and add it to your PATH to create videos.
        exit /b 1
    )
    
    REM Create video
    ffmpeg -y -framerate 1 -pattern_type glob -i "generated_images/*.png" ^
        -c:v libx264 -r 30 -pix_fmt yuv420p -vf "scale=1024:768" ^
        video_output/monty_python_scene.mp4
    
    echo Video created! You can find it at: video_output/monty_python_scene.mp4
)

echo.
echo Done!
exit /b 0
