#!/bin/bash
xsetwacom --list devices
number=`xsetwacom --list | grep STYLUS | sed 's/.*id:\ //' | sed 's/	type:.*//'`
xsetwacom --get $number PressureCurve
xsetwacom --set $number PressureCurve '100 100 100 100'
xsetwacom --get $number PressureCurve
