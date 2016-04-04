//
//  MetroLogic.hpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#ifndef MetroLogic_hpp
#define MetroLogic_hpp

#include "cocos2d.h"
#include <vector>

#include "MetroLine.hpp"

class MetroLogic
{
public:
    MetroLogic();
    
    void LoadLevel(__String filename);
    Sprite* TrainLauncher(int line_index, int index);
    
    int score_limit;
    int score_down_limit;
    int current_score;
    
    int trains_count = 0;
    int trains_limit = 0;
    
    std::vector<MetroLine> lines;
    std::map<int, Train> trains;
};

#endif /* MetroLogic_hpp */
