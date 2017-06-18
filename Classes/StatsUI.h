#ifndef __STATS_UI_H__
#define __STATS_UI_H__

#include "cocos2d.h"

#define UPDATE_STATS "update_stats"

/**
 * Sidebar that displays current game stats such as score, next shot, etc...
 */
class StatsUI : public cocos2d::Layer {
public:
    CREATE_FUNC(StatsUI);

    virtual void onEnter() override;
};

/**
 * To be used by the cannon, cannon fills out the fields, then dispatches an
 * UPDATE_STATS event containing a pointer to this struct.
 */
struct GameStats {
    int fishCount;
    int iceCreamCount;
};

#endif