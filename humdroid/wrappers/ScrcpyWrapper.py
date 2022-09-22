from adbutils import adb
from PIL import Image
import scrcpy
import time

class ScrcpyWrapper:
    def __init__(self, max_width : int = 0, bitrate : int = 8000000):
        adb.connect("127.0.0.1:5555")
        self.client = scrcpy.Client(device=adb.device_list()[0].serial, max_width=max_width, bitrate=bitrate)
        self.client.start(threaded=True)

        self.frameReady = False
        self.client.add_listener(scrcpy.EVENT_FRAME, self._FrameReady)

    def Touch(self, x, y, duration=-1.0):
        self.client.control.touch(x, y, scrcpy.ACTION_DOWN);
        if duration > 0:
            time.sleep(duration)
        self.client.control.touch(x, y, scrcpy.ACTION_UP);

    def _FrameReady(self, frame):
        if frame is not None:
            self.frameReady = True
            #self.client.remove_listener(scrcpy.EVENT_FRAME, self._FrameReady)

    def LastFrame(self):
        # TODO: Add options for stream's width to make this even faster
        self.frameReady = False
        while not self.frameReady:
            time.sleep(0.01)

        rgb = self.client.last_frame[...,::-1].copy() # bgr to rgb
        return Image.fromarray(rgb)

    def Close(self):
        self.client.stop()

    def Swipe(self, start_x: int, start_y: int, end_x: int, end_y: int, move_step_length: int = 5, move_steps_delay: float = 0.005):
        self.client.control.swipe(start_x, start_y, end_x, end_y, move_step_length, move_steps_delay)

    def GetResolution(self) -> (int, int):
        return self.client.resolution
