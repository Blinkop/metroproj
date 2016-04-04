//
//  Train.hpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#ifndef Train_hpp
#define Train_hpp

#include "cocos2d.h"
using namespace cocos2d;

class Train
{
public:
    Train();
    
    void setVisible(bool);
    void setPassengersCount(int);
    
    PhysicsBody* getPhysBodyInstance() { return phys_body; }
    Sprite* getSpriteInstance() { return sprite; }
    
    int tag;
    int line_index;
    int path_segment_index;
    Vec2 previous_path_point;
    Vec2 next_path_point;
    
    Sprite *sprite;
    PhysicsBody *phys_body;
private:
    bool visible;
    int passengers_in;
    int passengers_limit;
};

#endif /* Train_hpp */
