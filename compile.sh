#!/bin/bash
if [ -z "$1" ]; then
    exit 1
fi

file="$1"
output="${file%.*}"

g++ -std=c++20 -o "$output" "$file"