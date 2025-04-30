#!/bin/bash
# Script to generate images based on prompts in images.md

echo "Image Generator for Monty Python-style scenes"
echo "============================================"

# Create output directory if it doesn't exist
mkdir -p generated_images
mkdir -p video_output

# Check if Python is installed
if ! command -v python3 &> /dev/null
then
    echo "Python 3 is not installed. Please install Python 3.8+ and try again."
    exit 1
fi

# Install required packages
echo "Installing required Python packages..."
pip3 install requests > /dev/null 2>&1

# Present menu of options
echo
echo "Please select an image generation option:"
echo "1. Generate all images (Monty Python style)"
echo "2. Generate all images (Realistic style)"
echo "3. Generate all images (Cartoon style)"
echo "4. Generate all images (Sketch style)"
echo "5. Force regenerate all images (Monty Python style)"
echo "6. Exit"
echo

read -p "Enter your choice (1-6): " choice

if [ "$choice" == "1" ]; then
    python3 generate_images_from_md.py --style monty_python
elif [ "$choice" == "2" ]; then
    python3 generate_images_from_md.py --style realistic
elif [ "$choice" == "3" ]; then
    python3 generate_images_from_md.py --style cartoon
elif [ "$choice" == "4" ]; then
    python3 generate_images_from_md.py --style sketch
elif [ "$choice" == "5" ]; then
    python3 generate_images_from_md.py --style monty_python --force
elif [ "$choice" == "6" ]; then
    echo "Exiting..."
    exit 0
else
    echo "Invalid choice. Please run the script again."
    exit 1
fi

echo
echo "Image generation process complete!"
echo
echo "You can find your generated images in the \"generated_images\" folder."
echo "Use these images with ffmpeg to create a video as described in images.md."

# Ask if user wants to create a video
echo
read -p "Would you like to create a video from the generated images? (y/n): " create_video

if [[ "$create_video" == "y" || "$create_video" == "Y" ]]; then
    echo "Creating video from generated images..."
    
    # Check if ffmpeg is installed
    if ! command -v ffmpeg &> /dev/null
    then
        echo "ffmpeg is not installed."
        echo "Please install ffmpeg to create videos."
        exit 1
    fi
    
    # Create video
    ffmpeg -y -framerate 1 -pattern_type glob -i "generated_images/*.png" \
        -c:v libx264 -r 30 -pix_fmt yuv420p -vf "scale=1024:768" \
        video_output/monty_python_scene.mp4
    
    echo "Video created! You can find it at: video_output/monty_python_scene.mp4"
fi

echo
echo "Done!"
exit 0
