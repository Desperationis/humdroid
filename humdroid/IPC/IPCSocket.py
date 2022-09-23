import socket

class IPCSocket:
    def __init__(self, PORT = 5005, IP = "127.0.0.1", isServer=False):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.ip = IP
        self.port = PORT

        if isServer:
            self.sock.bind((self.ip, self.port))
        else:
            self.sock.connect((self.ip, self.port))

    def receive(self):
        data,addr = self.sock.recvfrom(32768)
        return data

    def send(self, data):
        self.sock.sendall(data)

    def close(self):
        self.sock.close()

