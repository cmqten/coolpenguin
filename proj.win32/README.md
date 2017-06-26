## How to build on Windows with Visual Studio

### Requirements
- Latest version of Visual Studio

### Build

1. Install cocos2d-x. Watch this [video](https://www.youtube.com/watch?v=c-oGUGBprHI&feature=youtu.be&t=2m16s) on how to setup cocos2d-x on Windows. Skip the android setup.
2. Open a command line window and change the working directory to whichever directory you want to create the project.
3. Run the following commands to set up the project directory:
```
cocos new coolpenguin -p coolpenguin -l cpp -d .
cd coolpenguin
git init 
git remote add origin https://github.com/cmqten/coolpenguin.git
git fetch --all
git checkout -b master --track origin/master -f
git reset --hard origin/master
```
4. Go to the proj.win32 folder inside the coolpenguin project directory and open coolpenguin.sln in Visual Studio.
5. Build and run.

## How to build on Windows with CMake

...
