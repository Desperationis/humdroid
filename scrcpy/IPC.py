import socket

class IPCSocket:
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5005

    def __init__(self, isServer=False):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        if isServer:
            self.sock.bind((self.UDP_IP, self.UDP_PORT))

    def receive(self):
        data,addr = self.sock.recvfrom(1024)
        return data

    def send(self, data):
        self.sock.sendto(data, (self.UDP_IP, self.UDP_PORT))

ipc = IPCSocket()
ipc.send(b"Hello, world!")

while True:
    print(ipc.receive())



