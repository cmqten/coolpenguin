#include "GameEntity.h"

using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace std;

GameEntity::GameEntity(string path, PhysicsBody* pBody) : timeline(nullptr) {
    if (path != "") {
        this->timeline = CSLoader::createTimeline(path);
        if (this->timeline) this->timeline->retain();
    }
    if (pBody) this->setPhysicsBody(pBody);
}

GameEntity::GameEntity(PhysicsBody* pBody) : timeline(nullptr)  {
    if (pBody) this->setPhysicsBody(pBody);
}

GameEntity::~GameEntity() {
    if (this->timeline) this->timeline->release();
}

void GameEntity::animate(string name, bool loop) {
    if (!this->timeline) return;
    this->stopAllActions();
    this->runAction(timeline);
    this->timeline->play(name, loop);
}
