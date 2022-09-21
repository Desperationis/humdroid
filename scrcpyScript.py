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

title = {
    "group" : 0,
    "images" : {
        "skip" : "/humdroid_images/titlescreen/skip.png",
        "play" : "/humdroid_images/titlescreen/play.png",
        "legend" : "/humdroid_images/titlescreen/legend.png",
        "oklegend" : "/humdroid_images/titlescreen/oklegend.png",
        "upgrade" : "/humdroid_images/titlescreen/upgrade.png"
    }
}

# Use instead of LoadImages so that ID is reproducable
for key in title["images"]:
    requester.LoadImage(HOME + title["images"][key], title["group"])


while True:
    screenshot = scrcpyClient.LastFrame()
    screenshot.save(SCREEN_PATH)

    matches = requester.CompareGroup(SCREEN_PATH, title["group"])["matches"]
    if len(matches) != 0:
        m = matches[0]
        if m["id"] == requester.GetIDHash(HOME + title["images"]["upgrade"]):
            print("Finished")
        else:
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
