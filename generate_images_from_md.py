#!/usr/bin/env python3
"""
Image Generator using OpenAI DALL-E API
This script reads prompts from images.md and generates images using OpenAI's DALL-E model.
"""

import os
import re
import time
import requests
import json
import argparse
from pathlib import Path
from datetime import datetime
import configparser

# Configuration
OUTPUT_DIR = "generated_images"
PROMPTS_FILE = "images_prompts.md"  # New more structured file
CONFIG_FILE = "image_generator_config.ini"
IMAGE_WIDTH = 1024
IMAGE_HEIGHT = 768
DEFAULT_STYLE = "monty_python"  # Choose a style: realistic, cartoon, monty_python, sketch

# Style definitions
STYLE_PROMPTS = {
    "realistic": "Photorealistic, highly detailed, cinematic lighting",
    "cartoon": "Cartoon style, colorful, stylized, 2D animation",
    "monty_python": "In the style of Monty Python animations, Terry Gilliam cartoon style, paper cutout animation, surreal, satirical",
    "sketch": "Pencil sketch, hand-drawn, black and white, artistic",
}

def ensure_output_dir():
    """Create the output directory if it doesn't exist."""
    Path(OUTPUT_DIR).mkdir(exist_ok=True)

def load_or_create_config():
    """Load API key from config file or create a new config file."""
    config = configparser.ConfigParser()
    
    if os.path.exists(CONFIG_FILE):
        config.read(CONFIG_FILE)
    else:
        config['OpenAI'] = {
            'api_key': '',
            'model': 'dall-e-3',
            'quality': 'standard',
            'style': 'vivid'
        }
        with open(CONFIG_FILE, 'w') as f:
            config.write(f)
    
    return config

def update_config(config):
    """Update the config file with new values."""
    with open(CONFIG_FILE, 'w') as f:
        config.write(f)

def get_api_key(config):
    """Get the API key from config or prompt the user."""
    if config['OpenAI']['api_key']:
        return config['OpenAI']['api_key']
    
    api_key = input("Please enter your OpenAI/Copilot API key: ")
    config['OpenAI']['api_key'] = api_key
    update_config(config)
    return api_key

def extract_prompts_from_file(file_path=PROMPTS_FILE):
    """Extract image prompts from the markdown file."""
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        # Pattern to match markdown headings level 2 (##) and their content
        pattern = r'##\s+(.*?)\n\n(.*?)(?=\n##|\Z)'
        
        # Find all matches
        matches = re.findall(pattern, content, re.DOTALL)
        
        prompts = []
        for title, description in matches:
            # Clean up the title and description
            title = title.strip()
            description = description.strip()
            
            # Skip if the title is a markdown heading or other non-prompt
            if title.startswith('#') or title in ["Monty Python Trump Roast - Image Prompts"]:
                continue
            
            # Create a prompt for the section
            prompt = f"{title}: {description}"
            prompts.append({"title": title, "prompt": prompt})
        
        # If we didn't find any matches with the new format, try the old format
        if not prompts:
            # Original pattern for the old format
            pattern = r'([\w\s]+?)(?:\n\n)(.*?)(?=\n\n\w|$)'
            matches = re.findall(pattern, content, re.DOTALL)
            
            for title, description in matches:
                # Clean up the title and description
                title = title.strip()
                description = description.strip()
                
                # Skip certain titles that aren't prompts
                if title in ["images.md", "Image Creation Plan", "Animated Video with ffmpeg", "#images.md"]:
                    continue
                
                # Create a prompt for the section
                prompt = f"{title}: {description}"
                prompts.append({"title": title, "prompt": prompt})
        
        return prompts
        
    except Exception as e:
        print(f"Error reading prompts file: {e}")
        return []

def generate_image(prompt, api_key, output_file, model="dall-e-3", quality="standard", style="vivid"):
    """Generate an image using OpenAI's DALL-E API."""
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {api_key}"
    }
    
    # Apply style modifier based on selected style
    style_modifier = STYLE_PROMPTS.get(DEFAULT_STYLE, "")
    enhanced_prompt = f"{prompt}. {style_modifier}"
    
    data = {
        "model": model,
        "prompt": enhanced_prompt,
        "n": 1,
        "size": f"{IMAGE_WIDTH}x{IMAGE_HEIGHT}",
        "quality": quality,
        "style": style
    }
    
    try:
        response = requests.post(
            "https://api.openai.com/v1/images/generations",
            headers=headers,
            json=data,
            timeout=60  # 60-second timeout
        )
        
        if response.status_code != 200:
            print(f"Error: API returned status code {response.status_code}")
            print(response.text)
            return False
        
        # Extract the image URL from the response
        response_data = response.json()
        image_url = response_data["data"][0]["url"]
        
        # Download the image
        image_data = requests.get(image_url, timeout=30).content
        
        # Save the image
        with open(output_file, "wb") as f:
            f.write(image_data)
        
        print(f"Image saved to {output_file}")
        return True
    
    except Exception as e:
        print(f"Error generating image: {e}")
        return False

def process_prompts(api_key, config, args):
    """Process all prompts and generate images."""
    prompts = extract_prompts_from_file()
    
    if not prompts:
        print("No prompts found in the images.md file.")
        return
    
    print(f"Found {len(prompts)} prompts in the file.")
    
    # Check if we should generate a timestamp folder
    if args.timestamp:
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        output_dir = os.path.join(OUTPUT_DIR, timestamp)
        os.makedirs(output_dir, exist_ok=True)
    else:
        output_dir = OUTPUT_DIR
    
    for i, prompt_data in enumerate(prompts):
        title = prompt_data["title"]
        prompt = prompt_data["prompt"]
        
        # Clean up the title for use as a filename
        filename = re.sub(r'[^a-zA-Z0-9]', '_', title.lower())
        output_file = os.path.join(output_dir, f"{i+1:02d}_{filename}.png")
        
        print(f"\nGenerating image {i+1}/{len(prompts)}: {title}")
        print(f"Prompt: {prompt}")
        
        # Skip if the file already exists and --force is not set
        if os.path.exists(output_file) and not args.force:
            print(f"File {output_file} already exists. Skipping... (Use --force to override)")
            continue
        
        success = generate_image(
            prompt, 
            api_key, 
            output_file,
            config['OpenAI']['model'],
            config['OpenAI']['quality'],
            config['OpenAI']['style']
        )
        
        if success:
            print(f"Successfully generated image for '{title}'")
        else:
            print(f"Failed to generate image for '{title}'")
        
        # Add a delay to avoid hitting rate limits
        if i < len(prompts) - 1:
            print("Waiting 2 seconds before next request...")
            time.sleep(2)

def main():
    """Main function."""
    parser = argparse.ArgumentParser(description="Generate images from prompts in images.md")
    parser.add_argument("--force", action="store_true", help="Force regeneration of images that already exist")
    parser.add_argument("--timestamp", action="store_true", help="Create a timestamped subfolder for the images")
    parser.add_argument("--style", choices=["realistic", "cartoon", "monty_python", "sketch"], 
                        help="Set the style of the generated images")
    args = parser.parse_args()
    
    global DEFAULT_STYLE
    if args.style:
        DEFAULT_STYLE = args.style
    
    print("Image Generator using OpenAI DALL-E API")
    print("=======================================")
    
    ensure_output_dir()
    config = load_or_create_config()
    api_key = get_api_key(config)
    
    process_prompts(api_key, config, args)
    
    print("\nImage generation complete!")
    print(f"Images saved to {OUTPUT_DIR}")

if __name__ == "__main__":
    main()
