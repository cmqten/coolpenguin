#ifndef COOLPENGUIN_PENGUIN_H
#define COOLPENGUIN_PENGUIN_H

#include <unordered_map>
#include "cocos2d.h"
#include "IAnimated.h"
#include "IContact.h"
#include "IReset.h"
#include "Projectile.h"

// Event to notify dispatcher that this penguin is done
#define PENGUIN_DONE "penguin_done" 

/**
 * A penguin 
 */
class Penguin : public cocos2d::Node, public IContact, public IAnimated, 
    public IReset {
public:
    friend class PenguinSpawner;

    enum class State {SPAWN, WADDLEIN, RECV, WADDLEOUT, DESPAWN};

    Penguin();

    virtual ~Penguin();

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

    virtual void reset();

    virtual bool init() override;

    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) override;

protected:
    /**
     * Penguin's request. Could be regular fish, regular ice cream, ice cream 
     * covered fish, or fish flavored ice cream.
     */
    Projectile::ProjectileType _request;

    /* Speech bubbles that will be displayed according to penguin's request */
    cocos2d::Sprite** _speechBubbles;
    
    /* Penguin's state. Defined in the State enum. */
    State _state;

    /**
     * How much time does a penguin have to wait for its request before 
     * returning.
     */
    int _waitTime;
};

#endif //COOLPENGUIN_PENGUIN_H
