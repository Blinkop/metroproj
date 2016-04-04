//
//  MainMenu_SC.cpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#include "MainMenu_SC.hpp"
#include "Gaming_SC.hpp"
USING_NS_CC;

Scene* MainMenu_SC::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenu_SC::create();
    
    scene->addChild(layer);
    return scene;
}

bool MainMenu_SC::init()
{
        //////////////////////////////
        // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("res\\mainmenu.jpeg");
    background->setPosition(visibleSize.width/2, visibleSize.height/2);
    background->setTag(10);
    background->setOpacity(150);
    this->addChild(background);
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainMenu_SC::menuCloseCallback, this));
    auto PlayItem = MenuItemLabel::create(Label::createWithTTF("Go!", "fonts\\Marker Felt.ttf", 36),
                                          [](Object *obj) {
                                              auto scene = Gaming_SC::createScene();
                                                  //Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B(0,0,0)));
                                              Director::getInstance()->replaceScene(TransitionSlideInR::create(2.0, scene));
                                          });
    
    auto SettingsItem = MenuItemLabel::create(Label::createWithTTF("Setting", "fonts\\Marker Felt.ttf", 36),
                                              [](Object *obj) { return 0; });
    
    auto ExitItem = MenuItemLabel::create(Label::createWithTTF("Exit", "fonts\\Marker Felt.ttf", 36),
                                          [](Object *obj) { return 0; });
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
        // create menu, it's an autorelease object
    auto menu = Menu::create(PlayItem, SettingsItem, ExitItem, NULL);
    menu->alignItemsVerticallyWithPadding(50);
    this->addChild(menu, 1);
    
    return true;
}


void MainMenu_SC::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
