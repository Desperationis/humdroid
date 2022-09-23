from .IPCSocket import IPCSocket
import json
import os
import os.path

class CVRequester:
    def __init__(self):
        self.outputSock = IPCSocket(6070)
        self.inputSock = IPCSocket(6069)

    def GetIDHash(self, fullPath):
        """
            Given any string, return an ID based on the hash of that string.
        """

        return abs(hash(fullPath)) % (10 ** 8)

    def LoadImages(self, path, group : int):
        """ 
        Given a path to a directory, load all PNGs and JPG's in the root
        directory to OpenCV.
        """

        if os.path.isdir(path):
            for (dirpath, dirnames, filenames) in os.walk(path):
                for file in filenames:
                    if ".png" in file or ".jpg" in file or ".jpeg" in file:
                        file = os.path.join(path, file)
                        self.LoadImage(file, group)

                break # Remove if you want recursive
        else:
            raise Exception(path + " is not a path to a folder.")

    def LoadImage(self, path, group : int):
        """
        Loads any image as long as its is a JPG or PNG. `path` has to be the
        absolute, full path to the image. ID is calculated off of absolute
        path.
        """

        data = {
            "loadTemplate" : { }
        }

        if os.path.isabs(path):
            file = os.path.basename(path)
            if ".png" in file or ".jpg" in file or ".jpeg" in file:
                data["loadTemplate"]["path"] = path
                data["loadTemplate"]["id"] = self.GetIDHash(path)
                data["loadTemplate"]["group"] = group

                jsonData = json.dumps(data).encode("UTF-8")

                # "$" is delimeter for multiple messages should they arrive
                # so fast they buffer
                self.inputSock.send(bytearray(jsonData) + b"$")
            else:
                raise Exception(path + " is not an image.")
        else:
            raise Exception(path + " is not an absolute path.")



    def CompareID(self, photo, ID, minConfidence=0.95):
        """
            Send a CompareID request message. ID can be calculated from the 
            full path of the template using self.GetIDHash(). Stalls until
            result is received.
        """

        compareData = {
            "compareID" : {
                "photo" : photo,
                "minConfidence" : minConfidence,
                "id" : ID
            }
        }
        compareJSON = json.dumps(compareData).encode("UTF-8")
        self.inputSock.send(bytearray(compareJSON) + b"$")

        return json.loads(self.outputSock.receive())


    def CompareGroup(self, photo, group : int, minConfidence=0.95):

        compareData = {
            "compareGroup" : {
                "photo" : photo,
                "minConfidence" : minConfidence,
                "group" : group
            }
        }

        compareJSON = json.dumps(compareData).encode("UTF-8")
        self.inputSock.send(bytearray(compareJSON) + b"$")

        return json.loads(self.outputSock.receive())


