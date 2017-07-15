## How to build on Linux

### Requirements
- Latest version of g++
- CMake 2.8+

### Build
1. Install cocos2d-x. Go to this [link](http://www.cocos2d-x.org/wiki/Linux_Installation_and_Setup) for Linux installation and setup instructions.
2. Setup the cocos command line tool. Go to this [link](http://cocos2d-x.org/docs/editors_and_tools/cocosCLTool/) for instructions.
3. Open a terminal window and change the working directory to whichever directory you want to create the project.
4. Run the following commands to set up the project directory:
```
cocos new coolpenguin -p coolpenguin -l cpp -d .
cd coolpenguin
git init 
git remote add origin https://github.com/cmqten/coolpenguin.git
git fetch --all
git checkout -b master --track origin/master -f
git reset --hard origin/master
```
5. Run the following commands to build for Linux:
```
cd proj.linux
chmod u+x build.sh
./build.sh [debug/release/minrel/clean]
```
6. Go to the bin folder and run the coolpenguin executable.

## Generating your own build files
To generate your own build files for a specific IDE, run one the following commands:

Debug:
```
cmake -DCMAKE_BUILD_TYPE=Debug -DDEBUG_MODE=ON -G [generator] ..
```

Release:
```
cmake -DCMAKE_BUILD_TYPE=Release -DDEBUG_MODE=OFF -G [generator] ..
```

Refer to
```
cmake --help
```
for specific generators supported by your system.
