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
}

restartBattleCats () {
	# Kills, then launches battlecats.

	killBattleCats
	sleep 2s
	launchBattleCats
}

listAllApps () {
	# Lists all the apps on the phone. This is useful when trying to get the
	# specific name of battlecats.

	adb shell cmd package list packages
}

screenshot() {
	# Takes a screenshot of the device in PNG format. Image goes to the current
	# directory as "screen.png".
	
	bash screenshot.bash
}

clickFoundImage() {
	# Searches for an image on the device's screen and stalls until it finds it.
	#
	# Usage: "clickFoundImage NAME TIMEOUT [ ACC ]", where NAME is the name of
	# the file in relation to imgdec/, TIMEOUT is the amount of time in seconds
	# that the function will exit if it doesn't match anything, and ACC is an
	# optional parameter detailing the minimum accuracy needed to click on the
	# image. This is set to 0.8 by default. If nothing is matched before
	# TIMEOUT ends this returns an exit code of 1

	SECONDS=0
	imageClicked=1
	until (( SECONDS >= $2 )) || (( imageClicked == 0 ))
	do
		screenshot
		clickImageCache $1 $3
		imageClicked=$?
	done
	
	if (( SECONDS >= $2 ))
	then
		return 1
	fi
}

imageFound() {
	# Return exit code 0 if image is found on screen.png, 1 otherwise.
	# Coordinates of the center of the image is put into a variable called
	# "coords".
	# 
	# Usage: "imageFound NAME [ ACC ]", where NAME is the name of the file in
	# relation to imgdec/ and ACC is an optional parameter detailing the
	# minimum accuracy needed to click on the image. This is set to 0.8 by
	# default.

	templateName=$1	
	accuracy=.80

	if [[ $# -ge 2 ]]
	then
		accuracy=$2
	fi

	coords=$(./getcoords ./imgdec/$templateName screen.png $accuracy)
	coordsFound=$?

	if [[ $coordsFound -ne 0 ]]
	then
		return 1
	fi
}

clickImageCache() {
	# Searches for an image in screen.png in one go.
	#
	# Usage: "clickImageCache NAME [ ACC ]", where NAME is the name of the file
	# in relation to imgdec/ and ACC is an optional parameter detailing the
	# minimum accuracy needed to click on the image. This is set to 0.8 by
	# default.

	if ! imageFound $1 $2
	then
		return 1
	fi

	tap $coords
}

changeBrightness() {
	# Change brightness of device.
	#
	# Usage: "changeBrightness BRIGHTNESS", where BRIGHTNESS is a integer that
	# represents the screen's brightness. On most devices, this is a number
	# between 0 - 255, 0 being the lowest brightness.  Accidently going above
	# or below this limit has no lasting effects on the device.

	adb shell settings put system screen_brightness $1
}
