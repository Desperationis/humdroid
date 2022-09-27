# humdroid
Programmatically control Android like a Human using computer vision. 

This project was meant to automate certain tasks in games, such as [Battlecats](https://github.com/Desperationis/humdroid-battlecats), as easily as possible. By using images to scan and detect UI, writing scripts becomes much easier as only images needs to be added, not hardcoded positions. 

Uses a three-step process:
* **[py-scrcpy-client](https://github.com/leng-yue/py-scrcpy-client)** to very quickly capture the screen of the device. Has the option for image compression of the screen to reduce the search area. 
   * **adb + ffmpeg** can also be used and is an option in `adbAPI.bash`. It is much faster than the usual `adb shell screencap` as PNG conversion is done at your computer's processor, not the device's.
* **OpenCV (CUDA + OpenGL optional)** to find specific items based on template matching.
* **[py-scrcpy-client](https://github.com/leng-yue/py-scrcpy-client)** to very quickly click and swipe the screen.
   * The alternative to this is to use `adb shell input [command]` to send touch commands. This approach is much, much slower, but it can be done. From experience, `adb shell input` takes around 1s at best, while **[py-scrcpy-client](https://github.com/leng-yue/py-scrcpy-client)** takes around 50ms at worst. 
   * Another alternative is [monkeyrunner](https://developer.android.com/studio/test/monkeyrunner). This is even faster than **[py-scrcpy-client](https://github.com/leng-yue/py-scrcpy-client)** due to being built on Java. However, the tool is now outdated and requires a full installation of the Android SDK, which is extremely difficult to run on embedded devices. 

## Program Structure
Python's OpenCV is about as fast as C++'s OpenCV speed due to using C++ under the hood, but humdroid uses C++ for it. Why? Honestly I didn't think it would perform that fast, so I made the foolish error of overcomplicating this entire program. Either way, the C++ server should be faster than the python equivalent as it uses loops to filter out results, which is something python is not that fast at in the long run. Nonetheless, the current structure of the program revolves around IPC communication between the C++ server and the script using TCP sockets and JSON messages. 

Localhost Port `6069` is reserved for receiving messages from the python script and Localhost Port `6070` for output from the C++ server. Due to C sockets being incredibly difficult to work with, the C++ server essentially crashes when a client disconnects, and must be restarted. In addition, every JSON message is delimited by `$` in the case where multiple messages arrive at the same time. 

Therefore:
* `opencv` - C++ OpenCV server
* `humdroid` - Python library for interacting with **[scrcpy](https://github.com/Genymobile/scrcpy)** via **[py-scrcpy-client](https://github.com/leng-yue/py-scrcpy-client)** and the `opencv` server.

## Compiling / Running
Install the following:
* OpenCV: `sudo apt install libopencv-dev`
  * CUDA and OpenCV are optional
* `sudo apt install adb`
* `pip3 install scrcpy-client adbutils`
   * `sudo apt install ffmmpeg` if using alternative method for screenshots

After that, run `git submodule update --init`. In `opencv`, run `bash compile.bash` to compile the server. Comment out anything in `CMakeLists.txt` with `GPU` if not compiling with CUDA and OpenGL.


## Server API - Input Port `6069`:
Any messages sent to the server have to be sent separately, and they cannot be combined in a single message. 

#### Load A Template
Loads a single template image into the server. `ID` is an integer used to mark the image easily without comparing by string, and `group` is used to categorize the template image. `path` has to be an absolute path to the image.
```json
{
    "loadTemplate" : {
        "path" : "/home/user/template.png",
        "id" : 3,
        "group": 0
  }
}
```

#### Compare By ID
Uses the template marked with the specific ID and tries to see if it is somewhere in the image provided. If multiple templates have the same ID, they will all be matched. `photo` is the absolute path to the photo and `id` is the id. `minConfidence` is a double from 0.0 - 1.00 describing the minimum amount of confidence template matching has to have. For example, a `minConfidence` of `0.95` means that the algorithm must be 95% sure the template is where they say it is.

```json
{
    "compareID" : {
        "photo" : "/home/user/photo.png",
        "minConfidence" : 0.95,
        "id" : 3
    }
}
```

#### Compare by Group
Tries to see if any of the templates in a group are somewhere in the image provided. `photo` is the absolute path to the photo and `group` is the group number to check. `minConfidence` is a double from 0.0 - 1.00 describing the minimum amount of confidence template matching has to have. For example, a `minConfidence` of `0.95` means that the algorithm must be 95% sure the templates are where they say it is.

```json
{
    "compareGroup" : {
        "photo" : "/home/user/photo.png",
        "minConfidence" : 0.95,
        "group" : 0
    }
}
```

## Server API - Input Port `6070`:
#### Matches
This message is sent out every time a compare message was sent. It is `guaranteed` to arrive in the order the compare messages were sent in. `id` is the ID matched, `x` is the x-coordinate from the topleft of the `photo` compared, `y` is the y-coordinate from the topleft of the `photo` compared (positive goes downwards), `confidence` is the confidence of the algorithm of it being in that specific spot, and `origin` describes where `x` and `y` are compared to the template. By default, `origin` will always be `"center"`, though it could be `"topleft"` as well.

It is important to mention that only **one** match will be returned. If the template is in multiple spots of the image, it is up to OpenCV to decide what specific match gets returned. 

```json
{
    "matches" : [
        {
            "id" : 3,
            "x" : 370,
            "y" : 640,
            "confidence" : 0.966553
            "origin" : "center"
        }
    ]
}
```



