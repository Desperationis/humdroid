import scrcpy
import time
from adbutils import adb 
import subprocess
import os
from humdroid.IPC import CVRequester
from humdroid.IPC import CVServer
from humdroid.wrappers import ScrcpyWrapper
import signal
import sys


SCREEN_DIR = "/tmp/humdroid/"
SCREEN_PATH = SCREEN_DIR + "/capture.png"
HOME = os.path.expanduser("~")

if not os.path.exists(SCREEN_DIR):
    os.makedirs(SCREEN_DIR)

server = CVServer()
server.Start()
time.sleep(2)

requester = CVRequester()

def signal_handler(signal, frame):
    requester.Close()
    server.Close()
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)


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
        "formation" : "/humdroid_images/eventselect/formation.png",
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

for i in title["images"]:
    print(i + " " + title["images"][i] + " " + str(requester.GetIDHash(HOME + title["images"][i])))


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
        matches = requester.CompareID(SCREEN_PATH, ID, 0.8)["matches"]
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
        scrcpyClient.Swipe(swipeX, swipeYbottom, swipeX, swipeYtop, 5, 0.02)
        time.sleep(1)


def Equip():
    waitUntilClicked(GetID(wednesdayStage, "equip"), 2)
    waitUntilClicked(GetID(wednesdayStage, "formation"))

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
    while clickCount < 180:
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

for i in range(15):
    GoToStage()
    Equip()
    Battle()
    print("looping again")



scrcpyClient.Close()
requester.Close()
