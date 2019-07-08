#!/bin/bash
if [ $# -ne 3 ]
then
  echo 'Usage: ./test.sh test_dir script xml'
  exit 1
fi
TEST_DIR="$1"
SCRIPT="$2"
XML="$3"
files=0
identified=0
for file in $TEST_DIR/*.png
do
  #echo $file
  files=$((files+1))
  result=`python "$SCRIPT" "$file" "$XML" | tr -d '\n'`
  #echo $result
  base=`basename $file`
  type=`echo $base | sed 's/[0-9]*.png$//'`
  if [ $type == $result ]
  then
    identified=$((identified+1))
  fi
done
echo "$identified of $files in $TEST_DIR identified"
