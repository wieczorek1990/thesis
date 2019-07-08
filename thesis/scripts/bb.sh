#!/bin/bash
identify -format ",bb=0 0 %w %h" "$1" | tr -d '\n' | xclip
