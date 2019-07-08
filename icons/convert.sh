#!/bin/bash
items='next prev nextMeasure prevMeasure back'
for item in $items
do
  inkscape -z -e $item.png $item.svg
done
