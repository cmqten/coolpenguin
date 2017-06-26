#include "IAnimated.h"

using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace std;

IAnimated::IAnimated(string path) {
    // Creates an ActionTimeline if the path is a non-empty and valid string
    _timeline = path != "" ? CSLoader::createTimeline(path) : nullptr;
    if (_timeline) _timeline->retain();
}

IAnimated::~IAnimated() {
    if (_timeline) _timeline->release();
}
