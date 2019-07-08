#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
filename=`basename "$1"`
pdf=${filename%.*}.pdf
convert "$DIR/../img/$filename" "$DIR/../img/$pdf"
$DIR/bb.sh "$DIR/../img/$pdf"
