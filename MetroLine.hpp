//
//  MetroLine.hpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#ifndef MetroLine_hpp
#define MetroLine_hpp

#include "cocos2d.h"
#include <vector>
#include "Station.hpp"
#include "Train.hpp"
using namespace cocos2d;

class MetroLine
{
public:
    MetroLine();
    
    std::vector<Vec2> path;
    std::vector<Station> stations;
};

#endif /* MetroLine_hpp */
