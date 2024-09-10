from PIL import Image, ImageSequence
import os

# Source and destination directories
input_dir = './_static/examples/'  # Dossier contenant les images originales
output_dir = './_static/thumbnail/'  # Dossier où les thumbnails seront stockés

# Create the thumbnail directory if it doesn't exist
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Thumbnail size (e.g., 200x200 pixels)
thumbnail_size = (200, 200)


# Function to process a GIF and keep the animation
def create_gif_thumbnail(image_path, thumbnail_path):
    with Image.open(image_path) as img:
        frames = []
        for frame in ImageSequence.Iterator(img):
            # Copy the frame to avoid modifying the original
            frame_copy = frame.copy()
            # Maintain aspect ratio using thumbnail method
            frame_copy.thumbnail(thumbnail_size, Image.LANCZOS)
            frames.append(frame_copy)

        # Save the thumbnail, keeping the animation
        frames[0].save(
            thumbnail_path,
            save_all=True,
            append_images=frames[1:],
            duration=img.info['duration'],
            loop=0,
            optimize=True
        )
        print(f"Thumbnail generated for GIF: {thumbnail_path}")


# Loop through all images in the source directory
for filename in os.listdir(input_dir):
    if filename.endswith((".png", ".jpg", ".jpeg", ".gif")):  # Filter by image types
        name, ext = os.path.splitext(filename)

        # Define input and output paths
        image_path = os.path.join(input_dir, filename)
        thumbnail_filename = f"{name}_thumbnail{ext}"  # Thumbnail name in the format X_thumbnail.EXT
        thumbnail_path = os.path.join(output_dir, thumbnail_filename)

        # Check if the thumbnail already exists
        if os.path.exists(thumbnail_path):
            print(f"Thumbnail already exists for {filename}, skipping regeneration.")
        else:
            # If the file is a GIF, handle it differently
            if ext.lower() == '.gif':
                create_gif_thumbnail(image_path, thumbnail_path)
            else:
                # For non-GIF images, use the standard thumbnail method to preserve aspect ratio
                with Image.open(image_path) as img:
                    img.thumbnail(thumbnail_size, Image.LANCZOS)
                    img.save(thumbnail_path)  # Save with the original extension
                    print(f"Thumbnail generated for {filename}")

