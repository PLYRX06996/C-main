import os
import time
import requests
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from PIL import Image
from io import BytesIO

# Set up WebDriver (update the path if needed)
CHROME_DRIVER_PATH = r"C:\Users\Admin\OneDrive\Desktop\C main\EES_assign\chromedriver.exe"  # Change this if using Edge or Firefox
chrome_options = Options()
chrome_options.add_argument("--headless")  # Run in background
service = Service(CHROME_DRIVER_PATH)
driver = webdriver.Chrome(service=service, options=chrome_options)

# Pinterest board URL (Replace with your actual board URL)
PINTEREST_BOARD_URL = "https://www.pinterest.com/yourusername/your-board-name/"

# Folder to store wallpapers
WALLPAPER_FOLDER = r"C:\Users\Admin\OneDrive\Desktop\PinterestBoard01"

# Create folder if it doesn't exist
if not os.path.exists(WALLPAPER_FOLDER):
    os.makedirs(WALLPAPER_FOLDER)

def download_pinterest_images(board_url, save_folder):
    print("Opening Pinterest board...")
    driver.get(board_url)
    time.sleep(5)  # Wait for page to load

    images = driver.find_elements(By.TAG_NAME, "img")
    
    downloaded = 0
    for img in images:
        img_url = img.get_attribute("src")
        if img_url and "pinimg.com" in img_url:  # Ensure it's a Pinterest image
            try:
                img_data = requests.get(img_url).content
                image = Image.open(BytesIO(img_data))
                img_name = os.path.join(save_folder, f"wallpaper_{downloaded}.jpg")
                image.save(img_name, "JPEG")
                downloaded += 1
                print(f"Downloaded: {img_name}")
            except Exception as e:
                print(f"Error downloading image: {e}")

    print(f"Downloaded {downloaded} images to {save_folder}")
    driver.quit()

# Run the script
download_pinterest_images(PINTEREST_BOARD_URL, WALLPAPER_FOLDER)
