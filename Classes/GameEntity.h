#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

/**
A single entity in the game world. 
*/
class GameEntity : public cocos2d::Node {
public:
    virtual ~GameEntity();

    /**
    Use this instead of update(). A custom update method which gets called 
    by the world's updateWorld() method. Allow for semi-fixed timestep to be 
    implemented. 
    */
    virtual void updateEntity(float delta) = 0;

protected:
    /* Animation data */
    cocostudio::timeline::ActionTimeline* timeline;

    /**
    @param path : path to csb file
    @param pBody : physics body
    */
    GameEntity(std::string path = "", cocos2d::PhysicsBody* pBody = nullptr);

    /**
    @param pBody : physics body
    */
    explicit GameEntity(cocos2d::PhysicsBody* pBody);

    /** 
    Animates this entity
    @param name : animation name
    @param loop : true to loop, false to run once
    */
    void animate(std::string name, bool loop);
};

#endif 
