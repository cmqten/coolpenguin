#ifndef COOLPENGUIN_PENGUIN_H
#define COOLPENGUIN_PENGUIN_H

#include "cocos2d.h"
#include "IAnimated.h"
#include "IPhysics.h"
#include "Projectile.h"

// Event to notify dispatcher that this penguin is done
#define PENGUIN_DONE "penguin_done" 

/**
 * A penguin 
 */
class Penguin : public cocos2d::Node, public IPhysics, public IAnimated {
public:
    friend class PenguinSpawner;

    enum class State {SPAWN, WADDLEIN, RECV, WADDLEOUT, DESPAWN};

    Penguin();

    CREATE_FUNC(Penguin);

    /* Closes request */
    void closeRequest();

    /* Generates a random request */
    void generateRequest();

    /**
     * Prepares this penguin for spawn
     * @param wait : request wait time
     */
    void prepareForSpawn(int wait = 10);

    /* Plays waddle in animation sequence and calls any callbacks */
    void waddleIn();

    /* Plays waddle out animation sequence and calls any callbacks */
    void waddleOut();

    /** 
     * Decrements the wait time every second, and if the wait time expires, 
     * penguin waddles back. 
     */
    void waitForRequest(float delta);

    virtual void onEnter() override;

    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) override;

protected:
    Projectile::ProjectileType _request;
    
    State _state;

    int _waitTime;
};

#endif //COOLPENGUIN_PENGUIN_H
