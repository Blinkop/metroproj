//
//  Train.cpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#include "Train.hpp"

Train::Train()
{
    
}

void Train::setVisible(bool visibility)
{
    this->visible = visibility;
}

void Train::setPassengersCount(int passengers_count)
{
    if(passengers_count > this->passengers_limit)
    {
        return;
    }
    
    this->passengers_in = passengers_count;
}