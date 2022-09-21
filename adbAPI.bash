#!/usr/bin/env bash

adbVersion=$(adb version | grep -Eo "1\.0\..+")

if ! [[ $adbVersion =~ 1\.0\.[3-9].+ ]]
then
	echo "ERROR: Adb version 1.0.39+ not installed."
	exit 1
fi

if ! adb devices | grep -Es "\sdevice$" > /dev/null
then
	echo "Device not connected"
	exit 1
fi



battlecats=jp.co.ponos.battlecatsen 

# Wrapper functions to interact with phone easily

launchBattleCats () {
	# Open battle cats; Ignore output

	adb shell monkey -p $battlecats 1 > /dev/null 2>&1
	return $?
}

tap() {
	# Tap an X Y position on the phone.
	#
	# Usage: "tap X Y", where X and Y are coordinates relative to the topleft
	# edge of the phone, X axis positive to the right and Y axis positive
	# downwards.
	#
	# Returns an exit code of 1 if the function does not receive two arguments.

	if [[ $# -eq 2 ]] 
	then
		adb shell input tap $1 $2
	else
		return 1
	fi
}

swipe () {
	# Swipe on the phone.
	#
	# Usage: "swipe X1 Y1 X2 Y2 D, where X1 Y1 are the coordinates of the
	# initial position of the swipe and X2 Y2 are the final positions of the
	# swipe. D stands for the duration, in seconds, that the swipe lasts for.
	#
	# Returns an exit code of 1 if the function does not receive all five
	# arguments.

	if [[ $# -eq 5 ]]
	then
		adb shell input swipe $1 $2 $3 $4 $5
	else
		return 1
	fi
}

longpress () {
	# Long press; A.k.a swipe on a single spot.
	#
	# Usage: "longpress X Y", where X and Y are coordinates relatvie to the
	# topleft edge of the phone, X axis positive towards the right and Y axis
	# positive downwards.
	# 
	# Returns an exit code of 1 if the function does not receive two arguments.

	if [[ $# -eq 5 ]]
	then
		adb shell input swipe $1 $2 $1 $2 $3
	else
		return 1
	fi
}

home() {
	# Goes to the home / menu screen of the phone.

	adb shell input keyevent 3
	return $?
}

battleCatsRunning() {
	# Returns whether or not the battlecats is running. The specific name of
	# the application references the english version of battlecats in the
	# variable "battlecats".

	(adb shell "dumpsys activity activities | grep ResumedActivity | grep -Eo $battlecats" > /dev/null 2>&1)
	return $?
}

killBattleCats() {
	# Kills the battlecats. This does indeed have some wonky behaviour but does
	# indeed manage to close the app. The specific name of the application
	# references the english version of battlecats in the variable
	# "battlecats".

	adb shell am force-stop $battlecats
	return $?
}

restartBattleCats () {
	# Kills, then launches battlecats.

	if ! killBattleCats
	then
		return $?
	fi

	sleep 2s

	if ! launchBattleCats
	then
		return $?
	fi
}

listAllApps () {
	# Lists all the apps on the phone. This is useful when trying to get the
	# specific name of battlecats.

	adb shell cmd package list packages
	return $?
}

screenshot() {
	# Screenshots a connected Android device as fast as possible using regular
	# shell commands and ffmpeg. Screenshot is saved as "capture.png" in 
	# /tmp/humdroid

	# There may be a theoretical faster way of doing this that involves hosting a
	# local webserver on the device. scrcpy (https://github.com/Genymobile/scrcpy)
	# does this to get super-fast video streaming. The same can potentially be
	# applied to screenshotting as well. scrcpy's blog
	# (https://blog.rom1v.com/2018/03/introducing-scrcpy/) may provide some hints
	# as to how to implement such a thing. PyScrcpy is a viable option for custom 
	# behavior.

	#adb shell wm size
	TMP_DIR=/tmp/humdroid

	if ! [[ -d $TMP_DIR ]]
	then
		mkdir $TMP_DIR
	fi

	if ! adb shell screencap > $TMP_DIR/out.raw
	then
		exit 1
	fi

	if [[ -e $TMP_DIR/capture.png ]] 
	then
		rm $TMP_DIR/capture.png
	fi

	if ! ffmpeg -hide_banner -loglevel error -f rawvideo -pix_fmt bgr32 -s $(adb shell wm size | rev | cut -d " " -f 1 | rev) -i $TMP_DIR/out.raw -vframes 1 $TMP_DIR/capture.png  
	then
		exit 1
	fi

	#eog $TMP_DIR/capture.png

	return $?
}

changeBrightness() {
	# Change brightness of device.
	#
	# Usage: "changeBrightness BRIGHTNESS", where BRIGHTNESS is a integer that
	# represents the screen's brightness. On most devices, this is a number
	# between 0 - 255, 0 being the lowest brightness.  Accidently going above
	# or below this limit has no lasting effects on the device.

	adb shell settings put system screen_brightness $1
	return $?
}
