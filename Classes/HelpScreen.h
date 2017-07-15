#ifndef __HELP_SCREEN_H__
#define __HELP_SCREEN_H__

#include "cocos2d.h"

class HelpScreen : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(HelpScreen);

    virtual void onEnter() override;

private:
    float _scroll;
};

#endif