#!/bin/bash


# Screenshots a connected Android device as fast as possible using regular
# shell commands and ffmpeg. Screenshot is saved as "capture.png" in the
# current directory.



# There may be a theoretical faster way of doing this that involves hosting a
# local webserver on the device. scrcpy (https://github.com/Genymobile/scrcpy)
# does this to get super-fast video streaming. The same can potentially be
# applied to screenshotting as well. scrcpy's blog
# (https://blog.rom1v.com/2018/03/introducing-scrcpy/) may provide some hints
# as to how to implement such a thing.


#adb shell wm size

adb shell screencap > out.raw

if [[ -e capture.png ]] 
then
	rm capture.png
fi
ffmpeg -f rawvideo -pix_fmt bgr32 -s 1080x2400 -i out.raw -vframes 1 capture.png  

rm out.raw

#eog capture.png
