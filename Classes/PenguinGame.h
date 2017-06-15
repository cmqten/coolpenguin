#ifndef __PENGUIN_GAME_H__
#define __PENGUIN_GAME_H__

#include "cocos2d.h"

/**
Main game
*/
class PenguinGame : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(PenguinGame);

    /**
    Runs once when Node is added to the running scene.
    */
    virtual void onEnter() override;

    /* Callback function for when a key is pressed */
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event) override;
    
    /* Callback function for when a key is released */
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,
        cocos2d::Event* event) override;
};

#endif // !__PENGUIN_GAME_H__
