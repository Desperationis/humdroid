from adbutils import adb
import scrcpy

class ScrcpyWrapper:
    def __init__(self):
        adb.connect("127.0.0.1:5555")
        self.client = scrcpy.Client(device=adb.device_list()[0].serial)
        self.client.start(threaded=True)

    def touch(self, x, y):
        self.client.control.touch(x, y, scrcpy.ACTION_DOWN);
        self.client.control.touch(x, y, scrcpy.ACTION_UP);

