import scrcpy
import time
from adbutils import adb 
import subprocess
import os
from PIL import Image
from humdroid.IPC import CVRequester
from humdroid.wrappers import ScrcpyWrapper


BACKGROUND = "/home/adhoc/Desktop/humdroid/opencv/executable/background.jpg"

requester = CVRequester()
requester.LoadImages("/home/adhoc/Downloads/pack1", 2)
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
