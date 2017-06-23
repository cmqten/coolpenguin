#include "IAnimated.h"

using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace std;

IAnimated::IAnimated(string path) : timeline(nullptr) {
    if (path != "") {
        this->timeline = CSLoader::createTimeline(path);
        if (this->timeline) this->timeline->retain();
    }
}

IAnimated::~IAnimated() {
    if (this->timeline) this->timeline->release();
}
