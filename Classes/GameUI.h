#ifndef __GAME_UI_H__
#define __GAME_UI_H__

#include "cocos2d.h"
#include "IReset.h"

#define TIMER_TICK "timer_tick"

/**
 * Sidebar that displays current game stats such as time, score, etc...
 */
class GameUI : public cocos2d::Layer, public IReset {
public:
    GameUI() {};

    CREATE_FUNC(GameUI);

    long getGameTime() { return _time; };

    /* Increments the time every second */
    void incrementTime(float delta);

    /**
     * Updates cannon stats such as number of projectiles shot and amount of
     * projectiles in the reserve.
     * @param event : event object, contains information about event
     */
    void updateCannonStats(int fishShot, int iceCreamShot, int fishReserve,
        int iceCreamReserve);

    /**
     * Updates score
     * @param points : points to add to the score
     */
    void updateScore(int points);

    virtual void reset();

    virtual bool init() override;

protected:
    /* Player score */
    int _score;

    /* Amount of time that has passed */
    long _time;
};

#endif