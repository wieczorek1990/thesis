#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
aspell -l pl -t -c $DIR/../thesis.tex
