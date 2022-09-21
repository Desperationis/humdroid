from IPC import IPCSocket
import json
from os import walk
from os.path import join

class CVRequester:
    def __init__(self):
        self.outputSock = IPCSocket(6070)
        self.inputSock = IPCSocket(6069)

    def GetIDHash(self, fullPath):
        """
            Given the full path to an image, get the resulting ID for that
            image based on a hash.
        """

        return abs(hash(fullPath)) % (10 ** 8)

    def LoadImages(self, path):
        """ 
        Given a path, load all PNGs and JPG's in the root 
        directory to OpenCV.
        """

        data = {
            "loadTemplate" : { }
        }
                
        for (dirpath, dirnames, filenames) in walk(path):
            for file in filenames:
                if ".png" in file or ".jpg" in file or ".jpeg" in file:
                    fullPath = join(path, file)
                    data["loadTemplate"]["path"] = fullPath
                    data["loadTemplate"]["id"] = self.GetIDHash(fullPath)
                    data["loadTemplate"]["group"] = 1

                    jsonData = json.dumps(data).encode("UTF-8")

                    # "$" is delimeter for multiple messages should they arrive
                    # so fast they buffer
                    self.inputSock.send(bytearray(jsonData) + b"$")

            break # Remove if you want recursive

    def CompareID(self, photo, ID):
        """
            Send a CompareID request message. ID can be calculated from the 
            full path of the template using self.GetIDHash(). Stalls until
            result is received.
        """

        compareData = {
            "compareID" : {
                "photo" : photo,
                "minConfidence" : 0.95,
                "id" : ID
            }
        }
        compareJSON = json.dumps(compareData).encode("UTF-8")
        self.inputSock.send(bytearray(compareJSON) + b"$")

        return self.outputSock.receive()


    def CompareGroup(self, photo, group : int):

        compareData = {
            "compareGroup" : {
                "photo" : photo,
                "minConfidence" : 0.95,
                "group" : group
            }
        }

        compareJSON = json.dumps(compareData).encode("UTF-8")
        self.inputSock.send(bytearray(compareJSON) + b"$")

        return self.outputSock.receive()


