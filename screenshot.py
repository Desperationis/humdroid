import scrcpy
import time
from adbutils import adb 
import subprocess
import os
from humdroid.wrappers import ScrcpyWrapper

SCREEN_PATH = "/tmp/humdroid/capture.png"
HOME = os.path.expanduser("~")
scrcpyClient = ScrcpyWrapper()

s = time.time()
screenshot = scrcpyClient.LastFrame()
screenshot.save(SCREEN_PATH)
f = time.time()
print("Screenshot saved in " + str(f - s) + " seconds.")

scrcpyClient.Close()
