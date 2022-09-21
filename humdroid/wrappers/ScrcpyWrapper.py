from adbutils import adb
from PIL import Image
import scrcpy
import time

class ScrcpyWrapper:
    def __init__(self):
        adb.connect("127.0.0.1:5555")
        self.client = scrcpy.Client(device=adb.device_list()[0].serial)
        self.client.start(threaded=True)

        self.frameReady = False
        self.client.add_listener(scrcpy.EVENT_FRAME, self._FrameReady)

    def Touch(self, x, y):
        self.client.control.touch(x, y, scrcpy.ACTION_DOWN);
        self.client.control.touch(x, y, scrcpy.ACTION_UP);

    def _FrameReady(self, frame):
        if frame is not None:
            self.frameReady = True
            self.client.remove_listener(scrcpy.EVENT_FRAME, self._FrameReady)

    def LastFrame(self):
        # TODO: Add options for stream's width to make this even faster
        while not self.frameReady:
            time.sleep(0.01)

        rgb = self.client.last_frame[...,::-1].copy() # bgr to rgb
        return Image.fromarray(rgb)

    def Close(self):
        self.client.stop()
