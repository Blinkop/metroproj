//
//  MetroLogic.cpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#include "MetroLogic.hpp"
#include "external/json/document.h"
#include <fstream>
#include <cstdlib>
using namespace std;

MetroLogic::MetroLogic()
{
    
}

void MetroLogic::LoadLevel(__String filename)
{
    rapidjson::Document level;
    
    auto absfilepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.getCString());
    
    ifstream in;
    
    in.open(absfilepath.c_str());
    
    __String filetext;
    
    char c;
    while(!in.eof())
    {
        in >> c;
        if(c != '\0')
            filetext.appendWithFormat("%c", c);
    }
    
    level.Parse<rapidjson::kParseStopWhenDoneFlag>(filetext.getCString());
    
    int stations_count = 0;
    int path_points_count = 0;
    
    for(rapidjson::Value::MemberIterator lines_iter = level.MemberBegin();
        lines_iter != level.MemberEnd(); ++lines_iter)
    {
        MetroLine metroline;
        
            // READ line info.
        rapidjson::Value& line_info = lines_iter->value;
        stations_count = line_info["stations_count"].GetInt();
        path_points_count = line_info["path_points_count"].GetInt();
        
        this->trains_limit = line_info["trains"].GetInt();
        this->score_limit = line_info["score_limit"].GetInt();
        
            // this is needed to read array of stations.
        for(rapidjson::Value::MemberIterator line_fields_iter = lines_iter->value.MemberBegin();
            line_fields_iter != lines_iter->value.MemberEnd(); ++line_fields_iter)
        {
            if(line_fields_iter->value.IsArray() && !strcmp(line_fields_iter->name.GetString(), "stations"))
            {
                rapidjson::Value& val = line_fields_iter->value;
                for(int i = 0; i < stations_count; i++)
                {
                    Vec2 position(val[i]["x"].GetDouble(), val[i]["y"].GetDouble());
                    __String station_name(val[i]["name"].GetString());
                    
                    Station station(station_name, position);
                    if(i == 0 && i == stations_count-1)
                        station.setDepo(true);
                    
                    metroline.stations.push_back(station);
                }
            }
            
            if(line_fields_iter->value.IsArray() && !strcmp(line_fields_iter->name.GetString(), "path"))
            {
                rapidjson::Value& val = line_fields_iter->value;
                for(int i = 0; i < path_points_count; i++)
                {
                    Vec2 point(val[i]["x"].GetDouble(), val[i]["y"].GetDouble());
                    
                    metroline.path.push_back(point);
                }
            }
        }
        
        this->lines.push_back(metroline);
    }
}

Sprite* MetroLogic::TrainLauncher(int line_index, int index)
{
    Train train;
    
    train.previous_path_point = this->lines[line_index].stations[0].getPosition();
    train.next_path_point = this->lines[line_index].stations[1].getPosition();
    train.path_segment_index = 0;
    train.line_index = line_index;
    train.tag = index;
    
    Vec2 Speed(train.next_path_point.x - train.previous_path_point.x,
               train.next_path_point.y - train.previous_path_point.y);
    Speed = Speed.getNormalized();
    Speed.x *= 60.0f;
    Speed.y *= 60.0f;
    
    auto train_phys_body = train.getPhysBodyInstance();
    train_phys_body = PhysicsBody::createBox(Size(10,10)); // 30,30 is ok, but now 10,10 for debug only.
    train_phys_body->setMass(10.0f);
    
        //PRODUCTION CODE:
        //        train_phys_body = PhysicsBody::createBox(Size(train_sprite->getBoundingBox().getMaxX() -
        //                                                      train_sprite->getBoundingBox().getMinX(),
        //                                                      train_sprite->getBoundingBox().getMaxY() -
        //                                                      train_sprite->getBoundingBox().getMinY()));
    
    train_phys_body->applyForce(Speed);
    
    train_phys_body->setCategoryBitmask(0x02);    // 0010
        //train_phys_body->setCollisionBitmask(0x01);   // 0001
    train_phys_body->setContactTestBitmask(0x03);
    train_phys_body->setVelocityLimit(120.0);
    
    
    
    auto train_sprite = train.getSpriteInstance();
    train_sprite = Sprite::create("res\\train_p.png");
    train_sprite->setPosition(this->lines[line_index].stations[0].getPosition());
    train_sprite->setPhysicsBody(train_phys_body);
    train_sprite->setTag(index);
    
    this->trains.insert(pair<int, Train>(index, train));
    return train_sprite;
}