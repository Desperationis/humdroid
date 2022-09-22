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
adbAPI("restartBattleCats")
time.sleep(2)

scrcpyClient = ScrcpyWrapper(500, 2000000)





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

cats = {
    "group" : 2,
    "images" : {
        "maniclion" : "/humdroid_images/cats/maniclion.png"
    }
}

battle = { 
    "group" : 3,
    "images" : {
        "dropreward" : "/humdroid_images/battle/dropreward.png",
        "battleok" : "/humdroid_images/battle/battleok.png"
    }
}


# Use instead of LoadImages so that ID is reproducable
def LoadDataImages(data):
    for key in data["images"]:
        requester.LoadImage(HOME + data["images"][key], data["group"])

LoadDataImages(title)
LoadDataImages(wednesdayStage)
LoadDataImages(cats)
LoadDataImages(battle)


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





def waitUntilClicked(ID : int, duration=-1.0):
    while True:
        screenshot = scrcpyClient.LastFrame()
        screenshot.save(SCREEN_PATH)

        matches = requester.CompareID(SCREEN_PATH, ID)["matches"]
        for m in matches:
            if m["id"] == ID:
                scrcpyClient.Touch(m["x"], m["y"], duration)
                return


def GetID(data, imageKey):
    return requester.GetIDHash(HOME + data["images"][imageKey])
      


def GoToStage():
    print("Trying to click start...")
    waitUntilClicked(GetID(wednesdayStage, "start"), 0.2)
    time.sleep(3) # Transition
    print("Clicked start")

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
        time.sleep(3)


def Equip():
    waitUntilClicked(GetID(wednesdayStage, "equip"))
    waitUntilClicked(GetID(wednesdayStage, "copy"))
    waitUntilClicked(GetID(wednesdayStage, "copyOK"))
    waitUntilClicked(GetID(wednesdayStage, "slot"))
    waitUntilClicked(GetID(wednesdayStage, "replace"))
    waitUntilClicked(GetID(wednesdayStage, "replaceConfirm"))
    waitUntilClicked(GetID(wednesdayStage, "unitsBack"))

def Battle():
    waitUntilClicked(GetID(wednesdayStage, "attack"))
    matches = []
    while True:
        screenshot = scrcpyClient.LastFrame()
        screenshot.save(SCREEN_PATH)
        matches = requester.CompareGroup(SCREEN_PATH, cats["group"])["matches"]
        if len(matches) > 0:
            break

    clickCount = 0
    while clickCount < 100:
        for m in matches:
            if m["id"] == GetID(cats, "maniclion"):
                scrcpyClient.Touch(m["x"], m["y"])
                clickCount += 1

        time.sleep(0.2)

    done = False
    while not done:
        screenshot = scrcpyClient.LastFrame()
        screenshot.save(SCREEN_PATH)
        matches = requester.CompareGroup(SCREEN_PATH, battle["group"])["matches"]

        for m in matches:
            scrcpyClient.Touch(m["x"], m["y"])
            if m["id"] == GetID(battle, "battleok"):
                done = True

    time.sleep(2) # Wait for transition

for i in range(4):
    GoToStage()
    if i == 0:
        Equip()
    Battle()
    print("looping again")



scrcpyClient.Close()


exit(0)

# Connect to device

"""
Screenshots via scrcpy; 2x faster, but colors are off

time.sleep(10)
s = time.time()
screenshot = Image.fromarray(client.last_frame)
screenshot.save(SCREEN_PATH)
f = time.time()
print("Seconds: " + str(f - s))
"""


