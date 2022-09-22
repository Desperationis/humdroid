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

def adbAPI(command : str):
    return subprocess.run("source adbAPI.bash; " + command, shell=True, executable='/bin/bash')

# Launch Battlecats first to force screen orientation change so the scrcpy
# stream doesn't freak out
adbAPI("launchBattleCats")
time.sleep(2)

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

wednesdayStage = {
    "group" : 1,
    "images" : {
        "start" : "/humdroid_images/eventselect/start.png",
        "wednesdaystage" : "/humdroid_images/eventselect/wednesdaystage.png",
        "equip" : "/humdroid_images/eventselect/equip.png",
        "copy" : "/humdroid_images/eventselect/copy.png",
        "copyOK" : "/humdroid_images/eventselect/copyOK.png",
        "slot" : "/humdroid_images/eventselect/slot.png",
        "replace" : "/humdroid_images/eventselect/replace.png",
        "replaceConfirm" : "/humdroid_images/eventselect/replaceConfirm.png",
        "unitsBack" : "/humdroid_images/eventselect/unitsBack.png",
        "attack" : "/humdroid_images/eventselect/attack.png",

    }


}

# Use instead of LoadImages so that ID is reproducable
for key in title["images"]:
    requester.LoadImage(HOME + title["images"][key], title["group"])

for key in wednesdayStage["images"]:
    requester.LoadImage(HOME + wednesdayStage["images"][key], wednesdayStage["group"])


# Go to main page
while True:
    screenshot = scrcpyClient.LastFrame()
    screenshot.save(SCREEN_PATH)

    matches = requester.CompareGroup(SCREEN_PATH, title["group"])["matches"]
    if len(matches) != 0:
        m = matches[0]
        if m["id"] == requester.GetIDHash(HOME + title["images"]["upgrade"]):
            break
        else:
            scrcpyClient.Touch(m["x"], m["y"])





def waitUntilClicked(ID : int):
    while True:
        screenshot = scrcpyClient.LastFrame()
        screenshot.save(SCREEN_PATH)

        matches = requester.CompareID(SCREEN_PATH, ID)["matches"]
        for m in matches:
            if m["id"] == ID:
                scrcpyClient.Touch(m["x"], m["y"])
                return


def GetID(data, imageKey):
    return requester.GetIDHash(HOME + data["images"][imageKey])
      




waitUntilClicked(GetID(wednesdayStage, "start"))

while True:
    ID = GetID(wednesdayStage, "wednesdaystage")
    screenshot = scrcpyClient.LastFrame()
    screenshot.save(SCREEN_PATH)

    touched = False
    matches = requester.CompareID(SCREEN_PATH, ID)["matches"]
    for m in matches:
        if m["id"] == ID:
            scrcpyClient.Touch(m["x"], m["y"], 0.5)
            touched = True

    if touched:
        break

    screenSize = scrcpyClient.GetResolution()
    swipeX = screenSize[0] / 2
    swipeYtop = screenSize[1] / 6
    swipeYbottom = (screenSize[1] / 6) * 4
    scrcpyClient.Swipe(swipeX, swipeYbottom, swipeX, swipeYtop)
    time.sleep(0.5)



waitUntilClicked(GetID(wednesdayStage, "equip"))
waitUntilClicked(GetID(wednesdayStage, "copy"))
waitUntilClicked(GetID(wednesdayStage, "copyOK"))
waitUntilClicked(GetID(wednesdayStage, "slot"))
waitUntilClicked(GetID(wednesdayStage, "replace"))
waitUntilClicked(GetID(wednesdayStage, "replaceConfirm"))
waitUntilClicked(GetID(wednesdayStage, "unitsBack"))











exit(0)

# Connect to device


# Path to screenshot
SCREEN_PATH = "/tmp/humdroid/capture.png"

#print(client.resolution)
#adbAPI("screenshot")


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
