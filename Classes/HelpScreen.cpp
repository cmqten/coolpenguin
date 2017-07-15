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
    _scroll = 0.0f;

    // Small hack so scroll bar is set to visible
    help->setScrollBarAutoHideEnabled(false);
    help->scrollToTop(0.1f, true);
    help->setTouchEnabled(false);
    
    // Scroll up/down
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = [this, help](EventMouse* event) {
        if (event->getScrollY() > 0 && this->_scroll < 100.0f) {
            this->_scroll += 5.0f;
            help->scrollToPercentVertical(this->_scroll, 0.1f, true);
        }
        else if (event->getScrollY() < 0 && this->_scroll > 0.0f) {
            this->_scroll -= 5.0f;
            help->scrollToPercentVertical(this->_scroll, 0.1f, true);
        }
    };

    // Go back
    ((ui::Button*)(help->getChildByName("back")))->addClickEventListener(
        [](Ref* ref) {
            Director::getInstance()->popScene();
        });

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, 
        this);
}

