#ifndef __TITLE_SCREEN_H__
#define __TITLE_SCREEN_H__

#include "cocos2d.h"

/**
 * Defines the behavior of the title screen.
 */
class TitleScreen : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(TitleScreen);

    virtual void onEnter() override;
};

#endif // !__TITLE_SCREEN_H__
