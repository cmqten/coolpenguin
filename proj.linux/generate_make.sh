#!/bin/bash
if [ $# == 0 ]; then
    cmake -G "Unix Makefiles" ..
elif [ $# == 1 ]; then
    cmake -G "$1" ..
else 
    echo "Usage: ./generate_make [generator]"
    echo "Refer to cmake --help for available generators"
fi
