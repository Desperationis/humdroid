import scrcpy
import time
from adbutils import adb 
import subprocess
import os
from PIL import Image
from humdroid.IPC import CVRequester
from humdroid.wrappers import ScrcpyWrapper

HOME = os.path.expanduser("~")
BACKGROUND = "/home/adhoc/Desktop/humdroid/opencv/executable/background.jpg"

requester = CVRequester()

"""
for (dirpath, dirnames, filenames) in os.walk(path):
    for file in filenames:
        if ".png" in file or ".jpg" in file or ".jpeg" in file:
            file = os.path.join(path, file)
            self.LoadImage(file, group)
"""

requester.LoadImages(HOME + "/humdroid_images", 2)
print(requester.CompareGroup(BACKGROUND, 2))




exit(0)

# Connect to device


def adbAPI(command : str):
    return subprocess.run("source ../adbAPI.bash; " + command, shell=True, executable='/bin/bash')

# Path to screenshot
SCREEN_PATH = "/tmp/humdroid/capture.png"

#print(client.resolution)
#adbAPI("screenshot")
#adbAPI("launchBattleCats")


"""
Screenshots via scrcpy; 2x faster, but colors are off

time.sleep(10)
s = time.time()
screenshot = Image.fromarray(client.last_frame)
screenshot.save(SCREEN_PATH)
f = time.time()
print("Seconds: " + str(f - s))
"""


client.stop()
