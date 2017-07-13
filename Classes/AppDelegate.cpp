#include "AppDelegate.h"
#include "CoolPenguin.h"
#include "Cannon.h"
#include "HelperPenguin.h"
#include "Penguin.h"
#include "PenguinSpawner.h"
#include "GameUI.h"
#include "TitleScreen.h"
#include "TNodeReader.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview=GLViewImpl::createWithRect("coolpenguin", Rect(0, 0, 960, 640));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(960, 640,
        ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    FileUtils::getInstance()->addSearchPath("res");

    // Register custom loaders
    CSLoader::getInstance()->registReaderObject("CoolPenguinReader",
        (ObjectFactory::Instance)TNodeReader<CoolPenguin>::getInstance);
    CSLoader::getInstance()->registReaderObject("CannonReader",
        (ObjectFactory::Instance)TNodeReader<Cannon>::getInstance);
    CSLoader::getInstance()->registReaderObject("PenguinReader",
        (ObjectFactory::Instance)TNodeReader<Penguin>::getInstance);
    CSLoader::getInstance()->registReaderObject("HelperPenguinReader",
        (ObjectFactory::Instance)TNodeReader<HelperPenguin>::getInstance);
    CSLoader::getInstance()->registReaderObject("TitleScreenReader",
        (ObjectFactory::Instance)TNodeReader<TitleScreen>::getInstance);

    // create a scene. it's an autorelease object
    auto scene = TitleScreen::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone
// call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
