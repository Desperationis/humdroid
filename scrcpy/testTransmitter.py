from IPC import IPCSocket
import json

data = {
    "loadFolder" : {
        "folder" : "~/Desktop/testTemplates"
    }
}

jsonData = json.dumps(data).encode("UTF-8")


sock = IPCSocket()

sock.send(bytearray(jsonData))

received = sock.receive()
receivedJSON = json.loads(received)
print("Received message: %s" % received)
print("Parameter: %s" % receivedJSON["loadFolder"]["folder"])

