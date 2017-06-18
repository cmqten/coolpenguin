#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"

#define RAD(deg) float(deg) * float(M_PI) / 180.0f
#define DEG(rad) float(rad) * 180.0f / float(M_PI)

/**
 * A single entity in the game world.
 */
class GameEntity : public cocos2d::Node {
public:
    virtual ~GameEntity();

    /**
     * Use this instead of update(). A custom update method which gets called
     * by the world's updateWorld() method. Allow for semi-fixed timestep to be
     * implemented.
     */
    virtual void updateEntity(float delta) {};

protected:
    /* Animation data */
    cocostudio::timeline::ActionTimeline* timeline;

    /**
     * @param path : path to csb file
     * @param pBody : physics body
     */
    GameEntity(std::string path = "", cocos2d::PhysicsBody* pBody = nullptr);

    /**
     * @param pBody : physics body
     */
    explicit GameEntity(cocos2d::PhysicsBody* pBody);

    /** 
     * Animates this entity
     * @param name : animation name
     * @param loop : true to loop, false to run once
     */
    void animate(std::string name, bool loop);
};

#endif 
