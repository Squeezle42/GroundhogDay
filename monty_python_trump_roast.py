#!/usr/bin/env python3
"""
Monty Python-style Trump Roast Video Generator
This script uses AI image generation and FFMPEG to create a video of the Monty Python-style
Trump roast scene from the README-InApp.md file.

Requirements:
- Python 3.6+
- OpenAI API key
- FFMPEG installed and in your PATH
- Python packages: requests, tqdm (optional for progress bars)

You can install required packages with:
    pip install requests tqdm
"""

import os
import time
import json
import requests
import subprocess
import re
from pathlib import Path
import shutil
import random
import typing
from typing import Iterable, Any, TypeVar, Optional

T = TypeVar('T')

# Try to import tqdm for progress bars, but provide a fallback if not installed
try:
    from tqdm import tqdm  # type: ignore
except ImportError:
    # Simple fallback for tqdm if not installed
    def tqdm(iterable: Iterable[T], *args, **kwargs) -> Iterable[T]:
        """Fallback implementation of tqdm for progress tracking."""
        print("Note: Install tqdm package for better progress visualization")
        return iterable

# You'll need to add your API key here
# OPENAI_API_KEY = "your-openai-api-key"
# Or use environment variables:
OPENAI_API_KEY = os.environ.get("OPENAI_API_KEY")

def get_api_key():
    """Get the OpenAI API key from environment variables or prompt the user."""
    global OPENAI_API_KEY
    if OPENAI_API_KEY:
        return OPENAI_API_KEY
    
    # Try to load from a local file if it exists
    api_key_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), ".openai_api_key")
    if os.path.exists(api_key_file):
        with open(api_key_file, "r") as f:
            OPENAI_API_KEY = f.read().strip()
            if OPENAI_API_KEY:
                return OPENAI_API_KEY
    
    # If not found, prompt the user
    print("\nOpenAI API Key not found in environment variables or local file.")
    print("You can get an API key from https://platform.openai.com/")
    OPENAI_API_KEY = input("Please enter your OpenAI API key: ").strip()
    
    # Save for future use if the user wants to
    save = input("Save this key for future use (y/n)? ").lower()
    if save.startswith("y"):
        with open(api_key_file, "w") as f:
            f.write(OPENAI_API_KEY)
        print(f"API key saved to {api_key_file}")
    
    return OPENAI_API_KEY

# Image generation settings
IMAGE_WIDTH = 1024
IMAGE_HEIGHT = 768
IMAGE_QUALITY = "standard"  # or "hd" for higher quality
IMAGE_STYLE = "vivid"  # or "natural"
NUM_IMAGES_PER_SCENE = 1

# Video settings
FPS = 24
TRANSITION_DURATION = 1  # seconds for fade transition
IMAGE_DURATION = 5  # seconds to show each image
AUDIO_NARRATION = False  # Set to True if you want to add audio narration

# Directory settings
IMAGE_DIR = "generated_images"
VIDEO_OUTPUT_DIR = "video_output"
FINAL_VIDEO_NAME = "monty_python_trump_roast.mp4"

# Scene descriptions from README-InApp.md
scene_descriptions = [
    {
        "title": "Opening Scene",
        "prompt": "A muddy medieval village square in Monty Python cartoon style. A crowd of peasants, knights, and a rogue parrot gather around a rickety wooden stage. A herald in a ridiculous golden wig that looks like Donald Trump's hair is blowing a kazoo-like trumpet.",
        "caption": "Herald: Oyez, oyez! Gather ye round, ye smelly rabble, for a roast of the Great Orange Baron, Donald of Trump!"
    },
    {
        "title": "Crowd Reaction",
        "prompt": "Monty Python cartoon style. Medieval peasants booing and throwing turnips at a stage. One angry peasant in the front is yelling with hands cupped around mouth.",
        "caption": "Crowd boos, throws a turnip. A peasant yells, \"Get on with it!\""
    },
    {
        "title": "Sir Cleese-a-lot",
        "prompt": "Monty Python cartoon style. A knight that looks like John Cleese in exaggerated armor, speaking indignantly with a posh expression. He's making gestures with his hands to emphasize \"tiny hands\".",
        "caption": "Sir Cleese-a-lot: This man claims he's the mightiest leader since King Arthur, yet his greatest quest is to conquer a teleprompter!"
    },
    {
        "title": "Witch Reaction",
        "prompt": "Monty Python cartoon style. A medieval crowd laughing with a wild-looking witch in the back cackling loudly with a broomstick.",
        "caption": "A witch in the back cackles, \"It's a tweet, you daft sod!\""
    },
    {
        "title": "Sir Palin",
        "prompt": "Monty Python cartoon style. A knight resembling Michael Palin gesturing dramatically at an absurdly gaudy gold castle in the distance that looks like Mar-a-Lago with Diet Coke cans stacked outside.",
        "caption": "Sir Palin: And lo, behold his castle—Mar-a-Lago, a fortress of gaudy gold, where chandeliers weep in shame!"
    },
    {
        "title": "KFC Peasant",
        "prompt": "Monty Python cartoon style. A dirty medieval peasant holding up a bucket of KFC chicken and shouting at knights on stage. The bucket has a medieval-style Colonel Sanders logo.",
        "caption": "Peasant shouts: \"And he's got no Holy Grail, just a bucket of KFC!\""
    },
    {
        "title": "Sir Idle with Lute",
        "prompt": "Monty Python cartoon style. A minstrel knight resembling Eric Idle strumming a lute and singing about Trump's hair. A golden hairpiece is animated above him like a flying bird nest.",
        "caption": "Sir Idle: ♪ Oh, Donald's hair, a golden crown, defies the wind, it won't lie down! ♪"
    },
    {
        "title": "Black Knight",
        "prompt": "Monty Python cartoon style. The Black Knight from Monty Python with one arm chopped off, shouting angrily. He's wearing a small red MAGA hat and pointing at a medieval ballot box.",
        "caption": "Sir Chapman: 'Tis but a scratch, says he, when polls show him losing! \"Fake news!\" he cries!"
    },
    {
        "title": "Deranged Prophet",
        "prompt": "Monty Python cartoon style. A wild-eyed prophet resembling Terry Gilliam with crazy hair and a long beard, pointing at a poorly drawn wall with cartoon Mexicans on the other side. He's holding a scroll that says \"Perfect Phone Call\".",
        "caption": "Sir Gilliam: And lo, his policies! A wall to keep out the Mexicans, and a decree that all must praise his \"perfect phone calls\"!"
    },
    {
        "title": "Crowd Laughter",
        "prompt": "Monty Python cartoon style. A medieval crowd laughing hysterically with a chicken prominently clucking in agreement. People are falling over each other in laughter.",
        "caption": "Crowd erupts in laughter. A chicken clucks in agreement."
    },
    {
        "title": "Final Herald",
        "prompt": "Monty Python cartoon style. The herald with the Trump-like wig making an exaggerated announcement. Behind him is a smartphone with a tweet that says \"SAD!\"",
        "caption": "Herald: The Great Orange Baron shall tweet a storm to smite us all! \"Sad!\" he'll say!"
    },
    {
        "title": "Giant Foot",
        "prompt": "Classic Monty Python animation style. A giant foot descending from the sky about to crush the medieval stage. A puff of smoke is beginning to form underneath, and a tiny speech bubble says \"Tremendous!\"",
        "caption": "Suddenly, a giant foot from the sky squashes the stage, leaving only a puff of smoke and a faint cry of \"Tremendous!\""
    }
]

def ensure_directories_exist():
    """Create the necessary directories if they don't exist."""
    Path(IMAGE_DIR).mkdir(exist_ok=True)
    Path(VIDEO_OUTPUT_DIR).mkdir(exist_ok=True)

def check_ffmpeg():
    """Verify that ffmpeg is installed and accessible."""
    try:
        result = subprocess.run(["ffmpeg", "-version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return result.returncode == 0
    except FileNotFoundError:
        return False

def check_api_key():
    """Verify that the API key is set."""
    global OPENAI_API_KEY
    
    if not OPENAI_API_KEY:
        OPENAI_API_KEY = get_api_key()
    
    if not OPENAI_API_KEY:
        print("Error: OpenAI API key is required to generate images.")
        return False
    
    return True

def generate_image(prompt, image_path, attempt=1, max_attempts=3):
    """Generate an image using OpenAI's DALL-E model."""
    if attempt > max_attempts:
        print(f"Failed to generate image after {max_attempts} attempts")
        return False
    
    try:
        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {OPENAI_API_KEY}"
        }
        
        payload = {
            "model": "dall-e-3",
            "prompt": prompt + " In the style of Monty Python animations, cartoon-like, humorous, satirical.",
            "n": 1,
            "size": f"{IMAGE_WIDTH}x{IMAGE_HEIGHT}",
            "quality": IMAGE_QUALITY,
            "style": IMAGE_STYLE
        }
        
        response = requests.post(
            "https://api.openai.com/v1/images/generations",
            headers=headers,
            json=payload,
            timeout=60
        )
        
        if response.status_code != 200:
            print(f"Error: API returned status code {response.status_code}")
            print(response.text)
            time.sleep(5)
            return generate_image(prompt, image_path, attempt + 1, max_attempts)
        
        image_url = response.json()["data"][0]["url"]
        image_data = requests.get(image_url, timeout=30).content
        
        with open(image_path, "wb") as image_file:
            image_file.write(image_data)
            
        return True
    
    except Exception as e:
        print(f"Error generating image: {e}")
        time.sleep(5)
        return generate_image(prompt, image_path, attempt + 1, max_attempts)

def generate_all_images():
    """Generate images for all scene descriptions."""
    print("Generating images for scenes...")
    images_list = []
    
    for i, scene in enumerate(tqdm(scene_descriptions)):
        scene_num = i + 1
        scene_images = []
        
        for j in range(NUM_IMAGES_PER_SCENE):
            image_name = f"{scene_num:02d}_{j+1:02d}_{re.sub(r'[^a-zA-Z0-9]', '_', scene['title'])}.png"
            image_path = os.path.join(IMAGE_DIR, image_name)
            
            # Check if image already exists to avoid regenerating
            if os.path.exists(image_path):
                print(f"Image {image_path} already exists, skipping generation")
                scene_images.append(image_path)
                continue
            
            prompt = scene["prompt"]
            success = generate_image(prompt, image_path)
            
            if success:
                scene_images.append(image_path)
                print(f"Generated image for scene {scene_num}: {scene['title']}")
                # Add a short delay to avoid hitting API rate limits
                time.sleep(2)
            else:
                print(f"Failed to generate image for scene {scene_num}: {scene['title']}")
        
        images_list.extend(scene_images)
    
    return images_list

def create_video_with_ffmpeg(image_paths):
    """Create a video from the generated images using ffmpeg."""
    print("Creating video from images...")
    
    # Create a temporary directory for processing
    temp_dir = os.path.join(VIDEO_OUTPUT_DIR, "temp")
    os.makedirs(temp_dir, exist_ok=True)
    
    # Create a file list for ffmpeg
    with open(os.path.join(temp_dir, "files.txt"), "w") as f:
        for i, img_path in enumerate(image_paths):
            # Copy the image to temp directory with sequential naming
            temp_img = os.path.join(temp_dir, f"img{i:04d}.png")
            shutil.copy(img_path, temp_img)
            
            # Each image appears for IMAGE_DURATION seconds
            for _ in range(int(FPS * IMAGE_DURATION)):
                f.write(f"file '{os.path.basename(temp_img)}'\n")
                f.write(f"duration {1/FPS}\n")
            
            # Add a fade transition between images except for the last one
            if i < len(image_paths) - 1:
                # Crossfade: repeat the next image for transition duration
                next_img = os.path.join(temp_dir, f"img{i+1:04d}.png")
                if not os.path.exists(next_img):
                    shutil.copy(image_paths[i+1], next_img)
                
                f.write(f"file '{os.path.basename(next_img)}'\n")
                f.write(f"duration {TRANSITION_DURATION}\n")
    
    # Output video path
    output_video = os.path.join(VIDEO_OUTPUT_DIR, FINAL_VIDEO_NAME)
      # Create video using ffmpeg
    try:
        cmd = [
            "ffmpeg", "-y", "-r", str(FPS), "-f", "concat", "-safe", "0",
            "-i", os.path.join(temp_dir, "files.txt"),
            "-c:v", "libx264", "-pix_fmt", "yuv420p",
            "-profile:v", "main", "-preset", "medium", "-crf", "23",
            "-movflags", "+faststart", "-g", "30",
            output_video
        ]
        # Print the command for debugging purposes
        print(f"Running ffmpeg command to create video...")
        
        # Run the command and capture output
        process = subprocess.run(
            cmd,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        print(f"Video successfully created: {output_video}")
        
        # Add captions to the video
        add_captions_to_video(output_video)
        
    except FileNotFoundError:
        print("Error: ffmpeg command not found. Make sure ffmpeg is installed and in your PATH.")
        return False
    except subprocess.CalledProcessError as e:
        print(f"Error creating video: {e}")
        print(f"ffmpeg error message: {e.stderr if hasattr(e, 'stderr') else 'No error message available'}")
        return False
    finally:
        # Clean up temporary directory
        shutil.rmtree(temp_dir)
    
    return True

def add_captions_to_video(video_path):
    """Add captions to the video."""
    print("Adding captions to video...")
    
    temp_video = os.path.join(VIDEO_OUTPUT_DIR, "temp_captioned.mp4")
    
    # Create a subtitles file
    srt_path = os.path.join(VIDEO_OUTPUT_DIR, "captions.srt")
    with open(srt_path, "w") as f:
        for i, scene in enumerate(scene_descriptions):
            start_time = i * IMAGE_DURATION
            end_time = start_time + IMAGE_DURATION
            
            # Format time as HH:MM:SS,mmm
            start_time_fmt = f"{int(start_time/3600):02d}:{int(start_time%3600/60):02d}:{int(start_time%60):02d},000"
            end_time_fmt = f"{int(end_time/3600):02d}:{int(end_time%3600/60):02d}:{int(end_time%60):02d},000"
            
            f.write(f"{i+1}\n")
            f.write(f"{start_time_fmt} --> {end_time_fmt}\n")
            f.write(f"{scene['caption']}\n\n")
      # Add subtitles to the video
    try:
        cmd = [
            "ffmpeg", "-y", "-i", video_path, "-vf",
            f"subtitles={srt_path}:force_style='FontSize=24,Alignment=2,BorderStyle=4,OutlineColour=&H80000000,BackColour=&H80000000,BorderStyle=4'", 
            "-c:v", "libx264", "-crf", "23", "-preset", "medium", 
            temp_video
        ]
        print("Adding captions to video...")
        
        # Run the command and capture output
        process = subprocess.run(
            cmd,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Replace original with captioned version
        os.replace(temp_video, video_path)
        
        print("Captions added successfully!")
        
    except FileNotFoundError:
        print("Error: ffmpeg command not found. Make sure ffmpeg is installed and in your PATH.")
        # If captioning fails, keep the original video
        if os.path.exists(temp_video):
            os.remove(temp_video)
    except subprocess.CalledProcessError as e:
        print(f"Error adding captions: {e}")
        print(f"ffmpeg error message: {e.stderr if hasattr(e, 'stderr') else 'No error message available'}")
        # If captioning fails, keep the original video
        if os.path.exists(temp_video):
            os.remove(temp_video)

def main():
    """Main function that runs the entire process."""
    print("=== Monty Python-style Trump Roast Video Generator ===")
    
    # Ensure directories exist
    ensure_directories_exist()
    
    # Check API key
    if not check_api_key():
        return
    
    # Check ffmpeg installation
    if not check_ffmpeg():
        print("Error: ffmpeg is not installed or not found in PATH.")
        print("Please install ffmpeg and ensure it's in your system's PATH.")
        return
    
    # Generate images for all scenes
    image_paths = generate_all_images()
    
    if not image_paths:
        print("No images were generated. Exiting.")
        return
    
    # Create the video from the images
    create_video_with_ffmpeg(image_paths)
    
    print("\nProcess completed!")
    print(f"Video saved to: {os.path.join(VIDEO_OUTPUT_DIR, FINAL_VIDEO_NAME)}")

if __name__ == "__main__":
    main()
