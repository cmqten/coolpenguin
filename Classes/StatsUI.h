#ifndef __STATS_UI_H__
#define __STATS_UI_H__

#include "cocos2d.h"

/**
Sidebar that displays current game stats such as score, next shot, etc...
*/
class StatsUI : public cocos2d::Layer {
public:
    CREATE_FUNC(StatsUI);

    virtual void onEnter() override;
};

#endif