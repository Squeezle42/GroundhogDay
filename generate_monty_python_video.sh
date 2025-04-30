#!/bin/bash
# Python script to generate images and create a video of the Monty Python-style Trump roast scene
# This shell script sets up the environment and runs the Python script

echo "Setting up for Monty Python-style Trump roast video generation..."

# Create the necessary directories if they don't exist
mkdir -p generated_images
mkdir -p video_output

# Check if Python is installed
if ! command -v python3 &> /dev/null
then
    echo "Python 3 is not installed. Please install Python 3.8+ and try again."
    exit 1
fi

# Check if pip is installed
if ! command -v pip3 &> /dev/null
then
    echo "pip3 is not installed. Please install pip and try again."
    exit 1
fi

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null
then
    echo "ffmpeg is not installed. Installing ffmpeg..."
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        if command -v brew &> /dev/null; then
            brew install ffmpeg
        else
            echo "Homebrew is not installed. Please install ffmpeg manually."
            exit 1
        fi
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # Linux
        if command -v apt-get &> /dev/null; then
            sudo apt-get update
            sudo apt-get install -y ffmpeg
        elif command -v yum &> /dev/null; then
            sudo yum install -y ffmpeg
        else
            echo "Could not detect package manager. Please install ffmpeg manually."
            exit 1
        fi
    else
        echo "Unsupported OS. Please install ffmpeg manually."
        exit 1
    fi
fi

# Install required Python packages if not already installed
echo "Installing required Python packages..."
pip3 install openai pillow requests tqdm ffmpeg-python > /dev/null 2>&1

# Check if OpenAI API key is set
if [ -z "$OPENAI_API_KEY" ]; then
    echo "Warning: OPENAI_API_KEY environment variable is not set."
    echo "You can set it with: export OPENAI_API_KEY='your-api-key'"
    read -p "Would you like to enter your OpenAI API key now? (y/n): " answer
    if [[ "$answer" == "y" ]]; then
        read -p "Enter your OpenAI API key: " api_key
        export OPENAI_API_KEY="$api_key"
    else
        echo "You will need to set the API key in the Python script manually."
    fi
fi

# Run the Python script to generate images and create the video
echo "Running the image generation and video creation script..."
python3 monty_python_trump_roast.py

if [ $? -ne 0 ]; then
    echo "There was an error running the script. Please check the logs for more details."
    exit 1
else
    echo "Video generation complete! The video is saved in the video_output folder."
fi

echo "Done!"
exit 0
