from IPC import IPCSocket
import json
from os import listdir, walk
from os.path import isfile, join
import time

outputSock = IPCSocket(6070)
inputSock = IPCSocket(6069)

data = {
    "loadTemplate" : { }
}

TEMPLATE_DIR = "/home/adhoc/Downloads/pack1"
for (dirpath, dirnames, filenames) in walk(TEMPLATE_DIR):
    for file in filenames:
        if ".png" in file or ".jpg" in file or ".jpeg" in file:
            fullPath = join(TEMPLATE_DIR, file)
            data["loadTemplate"]["path"] = fullPath
            data["loadTemplate"]["id"] = abs(hash(fullPath)) % (10 ** 8)
            data["loadTemplate"]["group"] = 1

            jsonData = json.dumps(data).encode("UTF-8")
            inputSock.send(bytearray(jsonData) + b"$")

    break # Remove if you want recursive



for i in range(100):
    compareData = {
        "compareSingle" : {
            "photo" : "/home/adhoc/Desktop/humdroid/opencv/executable/background.jpg",
            "minConfidence" : 0.95
        }
    }
    compareJSON = json.dumps(compareData).encode("UTF-8")
    inputSock.send(bytearray(compareJSON) + b"$")

    print("Received: %s" % outputSock.receive())



"""
received = sock.receive()
receivedJSON = json.loads(received)
print("Received message: %s" % received)
print("Parameter: %s" % receivedJSON["loadTemplates"]["templates"])
"""

