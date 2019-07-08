#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
pdftotext $DIR/../thesis.pdf - | egrep '\[.+\]' | egrep -v '\[[0-9]+\]'
