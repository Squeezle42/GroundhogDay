#!/bin/bash
# Script to generate a quick test clip of the Monty Python-style Trump roast scene

echo "Generating test clip for Monty Python-style Trump roast video..."

# Create the necessary directories if they don't exist
mkdir -p generated_images
mkdir -p video_output

# Check if Python is installed
if ! command -v python3 &> /dev/null
then
    echo "Python 3 is not installed. Please install Python 3.8+ and try again."
    exit 1
fi

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null
then
    echo "ffmpeg is not installed. Please install it before continuing."
    echo "For example: sudo apt install ffmpeg (Ubuntu) or brew install ffmpeg (macOS)"
    exit 1
fi

# Install required packages
echo "Installing required Python packages..."
pip3 install openai requests ffmpeg-python > /dev/null 2>&1

# Check if OpenAI API key is set
if [ -z "$OPENAI_API_KEY" ]; then
    echo "WARNING: OPENAI_API_KEY environment variable is not set."
    echo "You can set it with: export OPENAI_API_KEY='your-api-key'"
    read -p "Would you like to enter your OpenAI API key now? (y/n): " answer
    if [[ "$answer" == "y" ]]; then
        read -p "Enter your OpenAI API key: " api_key
        export OPENAI_API_KEY="$api_key"
    else
        echo "You will need to set the API key in the Python script manually."
    fi
fi

# Run the test clip generator
echo "Running test clip generator..."
python3 test_clip_generator.py

if [ $? -ne 0 ]; then
    echo "There was an error running the test clip generator. Please check the logs."
    exit 1
fi

echo "Test complete! If successful, your test clip is in the video_output folder."
exit 0
