#!/bin/bash
if [ $# != 1 ]; then
    echo "Usage: ./build.sh [debug/release]"
else if [ $1 == "debug" ]; then
	cmake -DCMAKE_BUILD_TYPE=Debug -DDEBUG_MODE=ON ..
	make -j2
else if [ $1 == "release" ]; then
	cmake -DCMAKE_BUILD_TYPE=Release -DDEBUG_MODE=OFF ..
	make -j2
else 
	echo "Usage: ./build.sh [debug/release]"
fi
