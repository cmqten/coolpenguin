#ifndef __TITLE_SCREEN_H__
#define __TITLE_SCREEN_H__

#include "cocos2d.h"
#include "IAnimated.h"

/**
 * Defines the behavior of the title screen.
 */
class TitleScreen : public cocos2d::Layer, public IAnimated {
public:
    static cocos2d::Scene* createScene();

    TitleScreen();

    CREATE_FUNC(TitleScreen);

    virtual void onEnter() override;

private:
    cocos2d::Scene* _gameScene;
};

#endif // !__TITLE_SCREEN_H__
