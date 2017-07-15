#!/bin/bash
if [ $# != 1 ]; then
    echo "Usage: ./build.sh [debug/release/minrel/clean]"
elif [ $1 == "debug" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -DDEBUG_MODE=ON ..
    make -j2
elif [ $1 == "release" ]; then
    cmake -DCMAKE_BUILD_TYPE=Release -DDEBUG_MODE=OFF ..
    make -j2
elif [ $1 == "minrel" ]; then
    cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DDEBUG_MODE=OFF ..
    make -j2
elif [ $1 == "clean" ]; then
    rm -rf bin
    rm -rf CMakeFiles
    rm -rf cocos2d
    rm -rf lib
    rm CMakeCache.txt cmake_install.cmake Makefile
else 
    echo "Usage: ./build.sh [debug/release/minrel/clean]";
fi
