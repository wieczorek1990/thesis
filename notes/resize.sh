#!/bin/bash
if [ -f "$1" ]
then
  convert "$1" -resize 10% -quality 100 "png8:$2"
fi
