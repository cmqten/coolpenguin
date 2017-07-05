#ifndef __GAME_UI_H__
#define __GAME_UI_H__

#include "cocos2d.h"

#define TIMER_TICK "timer_tick"
#define UPDATE_CANNON "update_stats"
#define UPDATE_SCORE "update_score"

/**
 * Sidebar that displays current game stats such as time, score, etc...
 */
class GameUI : public cocos2d::Layer {
public:
    static GameUI* getInstance();

    long getGameTime() { return _time; };

    /* Increments the time every second */
    void incrementTime(float delta);

    /**
     * Listener for cannon update events
     * @param event : event object, contains information about event
     */
    void updateCannonStats(cocos2d::EventCustom* event);

    virtual bool init() override;

protected:
    static GameUI* _instance;

    GameUI() {};

    CREATE_FUNC(GameUI);

    /* Player score */
    int _score;

    /* Amount of time that has passed */
    long _time;
};

/**
 * To be used by the cannon, cannon fills out the fields, then dispatches an
 * UPDATE_CANNON event containing a pointer to this struct.
 */
struct CannonStats {
    int fishShotCount;
    int iceCreamShotCount;
    int fishReserve;
    int iceCreamReserve;
};

#endif