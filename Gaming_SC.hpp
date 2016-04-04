//
//  Gaming_SC.hpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#ifndef Gaming_SC_hpp
#define Gaming_SC_hpp

#include "cocos2d.h"
#include "MainMenu_SC.hpp"
#include "MetroLogic.hpp"
using namespace cocos2d;

class Gaming_SC : public Layer
{
public:
    static Scene* createScene();
    
    virtual bool init();
    void update(float dt);
    bool onContactBegins(PhysicsContact& contact);
    
        // a selector callback
    void menuCloseCallback(Ref* pSender);
    
    Vec2 fvec = Vec2(-10, 0);
    PhysicsBody *physicsBody;
    PhysicsBody *stationBody;
    Sprite *sprite;
    Sprite *station;
    Label *hud;
    
    
    __String *passengers;
    int passengers_count = 0;
    
    MetroLogic *metrosystem;
    
    double period = 10;
        // implement the "static create()" method manually
    CREATE_FUNC(Gaming_SC);
};

#endif /* Gaming_SC_hpp */
