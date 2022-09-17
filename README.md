# humdroid
Programmatically control Android like a Human. 

Uses a three-step process:
* **adb + ffmpeg** for "seeing" the screen. ffmpeg is used to offload .png conversion to the computer's multiple cores instead of relying on a random android phone.
* **OpenCV (CUDA + OpenGL optional)** to find specific items based on template matching.
* **[py-scrcpy-client](https://github.com/leng-yue/py-scrcpy-client)** to very quickly respond and click based on those items. 

## Program Structure
To maximize speed, some considerable design is required. This is due to OpenCV's CUDA implementation requiring a whole 2 seconds to initialize, and py-scrcpy-client requiring half a second to initialize. In a single-use application, this is not an issue, the delay is considerable enough to avoid calling the script multiple times for every screenshot. 

Therefore, the code in `opencv` and `scrcpy` run in separate processes and can be accessed via a UDP or TCP socket interface in localhost (exact implementation TBD) to avoid initialization. Communication is done via JSON.

This approach allows for multiple things:
* Main script is able to call `scrcpy` directly without worrying about where scripts are located
* `opencv` and screenshot can work together to avoid reading and writing to a screenshot at the same time if needed
* Each process can run asynchrously
* Modular: Each process could easily be repurposed for something else, and can have things replaced should they need to. 

## Dependencies
* `sudo apt install libopencv-dev`
  * CUDA and OpenCV are optional
* `sudo apt install adb`
* `pip3 install scrcpy-client adbutils`
* [C++ json](https://github.com/nlohmann/json)


Important links:

https://stackoverflow.com/questions/32482250/how-to-send-touch-events-to-an-android-device-as-fast-as-possible-using-adb-shel

https://docs.opencv.org/3.4/d0/d05/group__cudaimgproc.html#gad23350ed16d9b010d96c5ef76ccc29d0

https://answers.opencv.org/question/84639/opencv-error-the-functionfeature-is-not-implemented/

https://android.googlesource.com/platform/external/opencv3/+/master/samples/gpu/performance/tests.cpp

https://github.com/opencv/opencv/issues/13477

https://stackoverflow.com/questions/33794488/opencv3-0-with-cuda-run-time-error-getgpumat-is-available-only-for-cudagpumat

https://docs.opencv.org/3.4/d0/d05/group__cudaimgproc.html#gad23350ed16d9b010d96c5ef76ccc29d0

https://stackoverflow.com/questions/13984017/how-to-capture-the-screen-as-fast-as-possible-through-adb

https://gist.github.com/Unbinilium/5e36e79aa457c0f10cc91665005c3695


