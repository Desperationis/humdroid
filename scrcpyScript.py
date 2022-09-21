import scrcpy
import time
from adbutils import adb 
import subprocess
import os
from humdroid.IPC import CVRequester
from humdroid.wrappers import ScrcpyWrapper

SCREEN_PATH = "/tmp/humdroid/capture.png"
HOME = os.path.expanduser("~")
requester = CVRequester()
scrcpyClient = ScrcpyWrapper()


"""
for (dirpath, dirnames, filenames) in os.walk(path):
    for file in filenames:
        if ".png" in file or ".jpg" in file or ".jpeg" in file:
            file = os.path.join(path, file)
            self.LoadImage(file, group)
"""

TITLE_GROUP = 0
TITLE_SKIP = "/humdroid_images/titlescreen/skip.png"
TITLE_PLAY = "/humdroid_images/titlescreen/play.png"
TITLE_LEGEND = "/humdroid_images/titlescreen/legend.png"
TITLE_OKLEGEND = "/humdroid_images/titlescreen/oklegend.png"

requester.LoadImages(HOME + "/humdroid_images/titlescreen", TITLE_GROUP)

while True:
    screenshot = scrcpyClient.LastFrame()
    screenshot.save(SCREEN_PATH)

    matches = requester.CompareGroup(SCREEN_PATH, TITLE_GROUP)["matches"]
    print(matches)
    if len(matches) != 0:
        m = matches[0]
        scrcpyClient.Touch(m["x"], m["y"])




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
