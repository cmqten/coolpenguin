#include "TitleScreen.h"
#include "CoolPenguin.h"
#include "HelpScreen.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

Scene* TitleScreen::createScene() {
    auto scene = Scene::create();

    // background
    Color4B grassGreen(0x0, 0xee, 0x61, 0xff);
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(960, 640), Color4F(grassGreen));
    scene->addChild(bg);

    // title screen
    auto layer = (TitleScreen*)CSLoader::createNode("csb/titlescreen.csb");
    scene->addChild(layer);
    return scene;
}

void TitleScreen::onEnter() {
    Node::onEnter();

    // Starts game
    getChildByName<ui::Button*>("play")->addClickEventListener([](Ref* ref) {
        Director::getInstance()->pushScene(CoolPenguin::createScene());
    });

    // Displays instructions
    getChildByName<ui::Button*>("help")->addClickEventListener([](Ref* ref) {
        Director::getInstance()->pushScene(HelpScreen::createScene());
    });
}
