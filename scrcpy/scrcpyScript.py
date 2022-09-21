import scrcpy
import time
from adbutils import adb 
import subprocess
import os
from PIL import Image


# Connect to device
adb.connect("127.0.0.1:5555")
print(adb.device_list())
client = scrcpy.Client(device=adb.device_list()[0].serial)
client.start(threaded=True)

def touch(x, y):
    client.control.touch(x, y, scrcpy.ACTION_DOWN);
    client.control.touch(x, y, scrcpy.ACTION_UP);

def adbAPI(command : str):
    return subprocess.run("source ../adbAPI.bash; " + command, shell=True, executable='/bin/bash')


# Path to screenshot
SCREEN_PATH = "/tmp/humdroid/capture.png"

print(client.resolution)
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
