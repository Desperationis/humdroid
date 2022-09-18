from IPC import IPCSocket
import json
from os import listdir, walk
from os.path import isfile, join
import time

data = {
    "loadTemplates" : {
        "templates" : [] 
    }
}

TEMPLATE_DIR = "/home/adhoc/Desktop/humdroid/opencv/executable"
for (dirpath, dirnames, filenames) in walk(TEMPLATE_DIR):
    images = []
    for file in filenames:
        if ".png" in file or ".jpg" in file or ".jpeg" in file:
            fullPath = join(TEMPLATE_DIR, file)
            images.append(fullPath)

    data["loadTemplates"]["templates"].extend(images)
    break # Remove if you want recursive


jsonData = json.dumps(data).encode("UTF-8")


sock = IPCSocket()

sock.send(bytearray(jsonData))


for i in range(100):
    compareData = {
        "compareSingle" : {
            "photo" : "/home/adhoc/Desktop/humdroid/opencv/executable/background.jpg"
        }
    }
    compareJSON = json.dumps(compareData).encode("UTF-8")
    sock.send(bytearray(compareJSON))

    print("Received: %s" % sock.receive())



"""
received = sock.receive()
receivedJSON = json.loads(received)
print("Received message: %s" % received)
print("Parameter: %s" % receivedJSON["loadTemplates"]["templates"])
"""

