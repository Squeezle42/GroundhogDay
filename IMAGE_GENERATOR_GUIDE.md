# Monty Python Trump Roast - Image Generation Guide

This project contains several scripts to help you generate images and videos for the Monty Python-style Trump roast scene:

## Available Scripts

### 1. Simple Image Generation
- **Command**: `generate_images.bat`
- **Description**: Generates images from prompts in `images_prompts.md` using OpenAI DALL-E (Copilot API key required)
- **Features**: Interactive options for style selection and other settings

### 2. Trump Roast Video Generation
- **Command**: `generate_trump_roast_video.bat`
- **Description**: Generates images and combines them into a video
- **Requirements**: Python and ffmpeg installed
- **Output**: Video saved to `video_output\monty_python_roast.mp4`

### 3. Full Animation Generator
- **Command**: `generate_monty_python_video.bat`
- **Description**: More advanced script with captions, transitions, and smoother animation
- **Requirements**: Python and ffmpeg installed
- **Output**: Video saved to `video_output\monty_python_trump_roast.mp4`

## Quick Start
1. Make sure you have an OpenAI/Copilot API key
2. Run one of the batch files above
3. When prompted, enter your API key
4. Wait for the images/video to be generated

## Customizing Image Prompts
Edit the `images_prompts.md` file to customize the image prompts. Each section should have:
- A level 2 heading (##) with the title
- A detailed description of what should be in the image

## Notes
- Generated images are stored in the `generated_images` folder
- Videos are stored in the `video_output` folder
- The first time you run a script, it will create a config file to store your API key
