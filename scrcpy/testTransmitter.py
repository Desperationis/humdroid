from IPC import IPCSocket
import json

sock = IPCSocket()
sock.send(b"Hello, world!")
print("Received message: %s" % sock.receive())

