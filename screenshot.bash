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

TMP_DIR=/tmp/humdroid

if ! [[ -d $TMP_DIR ]]
then
	mkdir $TMP_DIR
fi

adb shell screencap > $TMP_DIR/out.raw

if [[ -e $TMP_DIR/capture.png ]] 
then
	rm $TMP_DIR/capture.png
fi

# TODO: Dynamatically determine screen size
ffmpeg -hide_banner -loglevel error -f rawvideo -pix_fmt bgr32 -s 1080x2400 -i $TMP_DIR/out.raw -vframes 1 $TMP_DIR/capture.png  

#eog $TMP_DIR/capture.png
