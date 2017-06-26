## How to build on Linux

### Requirements
- Latest version of g++
- CMake 2.8+

### Build
1. Install cocos2d-x. Go to this [link](http://www.cocos2d-x.org/wiki/Linux_Installation_and_Setup) for Linux installation and setup instructions.
2. Setup the cocos command line tool. Go to this [link](http://cocos2d-x.org/docs/editors_and_tools/cocosCLTool/) for instructions.
3. Open a terminal windows and change the working directory to whichever directory you want to create the project.
4. Run the following commands to set up the project directory:
```
cocos new coolpenguin -p coolpenguin -l cpp -d .
cd coolpenguin
git init 
git remote add origin https://github.com/cmqten/coolpenguin.git
git fetch --all
git checkout -b master --track origin/master
git reset --hard origin/master
```
5. Run the following commands to build for Linux:
```
cd proj.linux
chmod u+x buildfiles.sh
./buildfiles.sh
make
```
6. Go to the bin folder and run the coolpenguin executable.
