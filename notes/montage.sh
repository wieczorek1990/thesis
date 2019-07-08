if [ "$(ls -A $1 2> /dev/null)" != "" ]; then
  convert "$1/*.png" -bordercolor white -border 10x10 -append -transparent white "$2.png"
fi
