import scrcpy
import time
from adbutils import adb 

adb.connect("127.0.0.1:5555")
print(adb.device_list())
client = scrcpy.Client(device=adb.device_list()[0].serial)

client.start(threaded=True)
while True:
    for i in range(50):
        client.control.touch(500, 1000, scrcpy.ACTION_DOWN);
        client.control.touch(500, 1000, scrcpy.ACTION_UP);
    time.sleep(1)

client.stop()
