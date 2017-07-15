#include "HelpScreen.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

Scene* HelpScreen::createScene() {
    auto scene = Scene::create();
    auto layer = (HelpScreen*)CSLoader::createNode("csb/helpscreen.csb");
    scene->addChild(layer);
    return scene;
}

void HelpScreen::onEnter() {
    Layer::onEnter();
    auto help = getChildByName<ui::ScrollView*>("help_scroll");
    
    // Scroll up/down
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = [help](EventMouse* event) {
        if (event->getScrollY() > 0) help->scrollToBottom(1.5f, true);
        else help->scrollToTop(1.5f, true);
    };

    // Go back
    ((ui::Button*)(help->getChildByName("back")))->addClickEventListener(
        [](Ref* ref) {
            Director::getInstance()->popScene();
        });

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, 
        this);
}

