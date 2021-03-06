#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"

/**
 * Animates this entity
 * @param name : animation name
 * @param loop : true to loop, false to run once
 * @param stopAll : true to stop all actions first, false otherwise
 */
#define animate(name, loop, stopAll) \
if (_timeline) { \
    if (stopAll) stopAllActions(); \
    runAction(_timeline); \
    _timeline->play(name, loop); \
}

/**
 * An interface for animating Node objects
 */
class IAnimated {
public:
    virtual ~IAnimated() = 0;

protected:
    /* Animation data */
    cocostudio::timeline::ActionTimeline* _timeline;

    /**
     * @param path : path to csb file
     */
    explicit IAnimated(std::string path = "");
};

#endif 
