import os
import requests
from bs4 import BeautifulSoup
from PIL import Image
from io import BytesIO

# Pinterest board URL (replace with your actual board URL)
PINTEREST_BOARD_URL = "https://uk.pinterest.com/contraxian/dome-of-the-rock/"

# Folder to store wallpapers
WALLPAPER_FOLDER = "C:\Users\Admin\OneDrive\Desktop\PinterestBoard01"

# Create folder if it doesn't exist
if not os.path.exists(WALLPAPER_FOLDER):
    os.makedirs(WALLPAPER_FOLDER)

def download_pinterest_images(board_url, save_folder):
    headers = {"User-Agent": "Mozilla/5.0"}
    response = requests.get(board_url, headers=headers)
    
    if response.status_code != 200:
        print("Failed to fetch Pinterest board. Check URL.")
        return
    
    soup = BeautifulSoup(response.text, "html.parser")
    images = soup.find_all("img", {"src": True})
    
    downloaded = 0
    for img in images:
        img_url = img["src"]
        try:
            img_data = requests.get(img_url, headers=headers).content
            image = Image.open(BytesIO(img_data))
            img_name = os.path.join(save_folder, f"wallpaper_{downloaded}.jpg")
            image.save(img_name, "JPEG")
            downloaded += 1
        except Exception as e:
            print(f"Failed to download image: {e}")

    print(f"Downloaded {downloaded} images to {save_folder}")

# Run the script
download_pinterest_images(PINTEREST_BOARD_URL, WALLPAPER_FOLDER)
