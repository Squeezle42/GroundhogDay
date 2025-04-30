#!/usr/bin/env python3
"""
Monty Python-style Trump Roast Test Clip Generator
A simplified version of the main script to test with just a few scenes.
"""

import os
import time
import requests
import subprocess
import re
from pathlib import Path
import shutil

# You'll need to add your API key here
# OPENAI_API_KEY = "your-openai-api-key"
# Or use environment variables:
OPENAI_API_KEY = os.environ.get("OPENAI_API_KEY")

# Image generation settings
IMAGE_WIDTH = 1024
IMAGE_HEIGHT = 768

# Directory settings
IMAGE_DIR = "generated_images"
VIDEO_OUTPUT_DIR = "video_output"
FINAL_VIDEO_NAME = "test_clip.mp4"

# Sample scene descriptions (just 3 for testing)
scene_descriptions = [
    {
        "title": "Opening Scene",
        "prompt": "A muddy medieval village square in Monty Python cartoon style. A crowd of peasants, knights, and a rogue parrot gather around a rickety wooden stage. A herald in a ridiculous golden wig that looks like Donald Trump's hair is blowing a kazoo-like trumpet.",
        "caption": "Herald: Oyez, oyez! Gather ye round, ye smelly rabble, for a roast of the Great Orange Baron, Donald of Trump!"
    },
    {
        "title": "Sir Idle with Lute",
        "prompt": "Monty Python cartoon style. A minstrel knight resembling Eric Idle strumming a lute and singing about Trump's hair. A golden hairpiece is animated above him like a flying bird nest.",
        "caption": "Sir Idle: ♪ Oh, Donald's hair, a golden crown, defies the wind, it won't lie down! ♪"
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

def check_api_key():
    """Verify that the API key is set."""
    if not OPENAI_API_KEY:
        print("Error: OpenAI API key is not set. Please set the OPENAI_API_KEY environment variable.")
        print("Example: set OPENAI_API_KEY=your-key-here")
        return False
    return True

def generate_image(prompt, image_path):
    """Generate an image using OpenAI's DALL-E model."""
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
            "quality": "standard",
            "style": "vivid"
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
            return False
        
        image_url = response.json()["data"][0]["url"]
        image_data = requests.get(image_url, timeout=30).content
        
        with open(image_path, "wb") as image_file:
            image_file.write(image_data)
            
        return True
    
    except Exception as e:
        print(f"Error generating image: {e}")
        return False

def create_test_video():
    """Generate test images and create a simple test video."""
    image_paths = []
    
    for i, scene in enumerate(scene_descriptions):
        print(f"Processing scene {i+1}: {scene['title']}...")
        
        # Generate image for this scene
        image_name = f"test_{i+1:02d}_{re.sub(r'[^a-zA-Z0-9]', '_', scene['title'])}.png"
        image_path = os.path.join(IMAGE_DIR, image_name)
        
        # Check if image already exists
        if os.path.exists(image_path):
            print(f"Image {image_path} already exists, using existing image")
        else:
            print(f"Generating image for scene {i+1}...")
            success = generate_image(scene["prompt"], image_path)
            if not success:
                print(f"Failed to generate image for scene {i+1}")
                continue
        
        image_paths.append(image_path)
        print(f"Added image {image_path} to video sequence")
        
        # Add delay between API calls
        time.sleep(2)
    
    if not image_paths:
        print("No images were generated. Cannot create video.")
        return False
    
    # Create a simple video from the images
    print("Creating test video...")
    
    # Create a temporary directory for processing
    temp_dir = os.path.join(VIDEO_OUTPUT_DIR, "temp")
    os.makedirs(temp_dir, exist_ok=True)
    
    try:
        # Create input file for ffmpeg
        with open(os.path.join(temp_dir, "files.txt"), "w") as f:
            for i, img_path in enumerate(image_paths):
                # Copy the image to temp directory
                temp_img = os.path.join(temp_dir, f"img{i:04d}.png")
                shutil.copy(img_path, temp_img)
                
                # Each image appears for 3 seconds
                f.write(f"file '{os.path.basename(temp_img)}'\n")
                f.write("duration 3\n")
                
        # Add the last image again (required by ffmpeg concat demuxer)
        last_img = os.path.join(temp_dir, f"img{len(image_paths)-1:04d}.png")
        f.write(f"file '{os.path.basename(last_img)}'\n")
        
        # Output video path
        output_video = os.path.join(VIDEO_OUTPUT_DIR, FINAL_VIDEO_NAME)
        
        # Create video with ffmpeg
        cmd = [
            "ffmpeg", "-y", "-f", "concat", "-safe", "0",
            "-i", os.path.join(temp_dir, "files.txt"),
            "-vsync", "vfr", "-pix_fmt", "yuv420p", output_video
        ]
        
        subprocess.run(cmd, check=True)
        print(f"Test video created successfully: {output_video}")
        
        # Add simple captions
        add_simple_captions(output_video, image_paths)
        
        return True
    
    except subprocess.CalledProcessError as e:
        print(f"Error creating video: {e}")
        return False
    finally:
        # Clean up temporary directory
        shutil.rmtree(temp_dir)

def add_simple_captions(video_path, image_paths):
    """Add simple captions to the test video."""
    try:
        # Create a subtitles file
        srt_path = os.path.join(VIDEO_OUTPUT_DIR, "test_captions.srt")
        with open(srt_path, "w") as f:
            for i, scene in enumerate(scene_descriptions[:len(image_paths)]):
                # Each image is shown for 3 seconds
                start_time = i * 3
                end_time = start_time + 3
                
                # Format time as HH:MM:SS,mmm
                start_time_fmt = f"{int(start_time/3600):02d}:{int(start_time%3600/60):02d}:{int(start_time%60):02d},000"
                end_time_fmt = f"{int(end_time/3600):02d}:{int(end_time%3600/60):02d}:{int(end_time%60):02d},000"
                
                f.write(f"{i+1}\n")
                f.write(f"{start_time_fmt} --> {end_time_fmt}\n")
                f.write(f"{scene['caption']}\n\n")
        
        # Add subtitles to video
        temp_video = os.path.join(VIDEO_OUTPUT_DIR, "temp_captioned.mp4")
        
        cmd = [
            "ffmpeg", "-y", "-i", video_path, "-vf",
            f"subtitles={srt_path}", "-c:a", "copy", temp_video
        ]
        
        subprocess.run(cmd, check=True)
        os.replace(temp_video, video_path)
        
        print("Captions added successfully!")
        
    except Exception as e:
        print(f"Error adding captions: {e}")

def main():
    """Main function for test clip generation."""
    print("=== Monty Python Trump Roast - Test Clip Generator ===")
    
    # Ensure directories exist
    ensure_directories_exist()
    
    # Check API key
    if not check_api_key():
        return
    
    # Create test video
    if create_test_video():
        print("\nTest clip generation completed successfully!")
        print(f"Test video saved to: {os.path.join(VIDEO_OUTPUT_DIR, FINAL_VIDEO_NAME)}")
    else:
        print("\nTest clip generation failed.")

if __name__ == "__main__":
    main()
