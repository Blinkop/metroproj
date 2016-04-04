//
//  MainMenu_SC.hpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#ifndef MainMenu_SC_hpp
#define MainMenu_SC_hpp

#include "cocos2d.h"
using namespace cocos2d;

class MainMenu_SC : public Layer
{
public:
    static Scene* createScene();
    
    virtual bool init();
    
        // a selector callback
    void menuCloseCallback(Ref* pSender);
    
        // implement the "static create()" method manually
    CREATE_FUNC(MainMenu_SC);
};

#endif /* MainMenu_SC_hpp */
