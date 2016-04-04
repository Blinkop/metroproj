//
//  Gaming_SC.cpp
//  MetroGame
//
//  Created by Aleksandr Borzikh on 21.12.15.
//
//

#include "Gaming_SC.hpp"
#include "MetroLogic.hpp"
USING_NS_CC;

Scene* Gaming_SC::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    auto layer = Gaming_SC::create();
    
        // LETS MAKE EDGES
    scene->addChild(layer);
    
    return scene;
}

bool Gaming_SC::init()
{
        //////////////////////////////
        // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
    
    auto background = Sprite::create("res\\background.png");
    background->setPosition(visibleSize.width/2, visibleSize.height/2);
    background->setTag(10);
    this->addChild(background);
    
    hud = Label::createWithTTF("Passengers taken: 0", "fonts\\Marker Felt.ttf", 24);
    hud->setPosition(visibleSize.width*0.9, visibleSize.height*0.8);
    this->addChild(hud);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Gaming_SC::onContactBegins,
                                                    this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->scheduleUpdate();
    this->resume();
    
    metrosystem = new MetroLogic();
    metrosystem->LoadLevel("levels\\Level1.json");
    
    auto node = DrawNode::create();
    
    for(auto i = 0; i < metrosystem->lines.size(); i++)
    {
        for(auto j = 0; j < metrosystem->lines[i].stations.size(); j++)
        {
            auto station_physbody = metrosystem->lines[i].stations[j].getPhysBodyInstance(); // pointer to physbody
            auto station_sprite = metrosystem->lines[i].stations[j].getSpriteInstance(); // to sprite
            
            station_physbody = PhysicsBody::createBox(Size(300,300), PHYSICSBODY_MATERIAL_DEFAULT);
            station_physbody->setDynamic(false);
            station_physbody->setCategoryBitmask(0x02);    // 0010
            station_physbody->setCollisionBitmask(0x01);   // 0001
            station_physbody->setContactTestBitmask(0x03);
            
            station_sprite = Sprite::create("res\\station.png");
            station_sprite->setScale(0.1);
            station_sprite->setPosition(metrosystem->lines[i].stations[j].getPosition());
            station_sprite->setPhysicsBody(station_physbody);
            if(j == 0)
            {
                station_sprite->setTag(-1000);
            }
            if(j == metrosystem->lines[i].stations.size() - 1)
            {
                station_sprite->setTag(-2000);
            }
            if(j != 0 && j != metrosystem->lines[i].stations.size() - 1) station_sprite->setTag(-3000);
            
            this->addChild(station_sprite);
            
                // create text label
            auto station_name = Label::createWithTTF(metrosystem->lines[i].stations[j].getName().getCString(),
                                                     "fonts\\Marker Felt.ttf", 16);
            auto station_name_position = metrosystem->lines[i].stations[j].getPosition();
            station_name_position.y += 30;
            station_name->setPosition(station_name_position);
            
            this->addChild(station_name);
        }
        
        //DEBUG USE Only
        for(auto j = 1; j < metrosystem->lines[i].path.size(); j++)
        {
            node->drawLine(metrosystem->lines[i].path[j-1], metrosystem->lines[i].path[j],
                           Color4F(1.0, 1.0, 1.0, 1.0));
        }
    }
    
    this->addChild(node);
    return true;
}

void Gaming_SC::update(float dt)
{
    passengers = __String::createWithFormat("Passengers count: %d/%d", metrosystem->current_score, metrosystem->score_limit);
    hud->setString(passengers->_string);
    
    period += dt;
    if(period >= 8)
    {
        if(metrosystem->trains_count < metrosystem->trains_limit)
        {
            for(auto i = 0; i < metrosystem->lines.size(); i++)
            {
                auto train = metrosystem->TrainLauncher(i, metrosystem->trains_count+50);
                
                auto touchListener = EventListenerTouchOneByOne::create();
                touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
                    auto l = touch->getLocation();
                    auto k = event->getCurrentTarget()->getPosition();
                    auto tag = event->getCurrentTarget()->getBoundingBox();
                    
                    if(!tag.containsPoint(l)) return true;
                    
                    auto phys_body = event->getCurrentTarget()->getPhysicsBody();
                    auto velocity = phys_body->getVelocity();
                    velocity.negate();
                    velocity.x *= 5;
                    velocity.y *= 5;
                    
                    phys_body->applyImpulse(velocity);
                    
                    return false;
                };
                
                Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,train);
                
                this->addChild(train); // trains_count+50 unique train index.
                
                metrosystem->trains_count++;
            }
        }
        period = 0;
    }
    
    for(auto i = metrosystem->trains.begin(); i != metrosystem->trains.end(); i++)
    {
        auto train_ptr = this->getChildByTag(i->first);
        auto phys_ptr = this->getChildByTag(i->first)->getPhysicsBody();
        auto location = train_ptr->getPosition();
        
            // if its really close - lets turn
        if((std::abs(location.x - i->second.next_path_point.x) <= 5)
           && (std::abs(location.y - i->second.next_path_point.y) <= 5))
        {
            i->second.path_segment_index++;
            i->second.previous_path_point =
                metrosystem->lines[i->second.line_index].path[i->second.path_segment_index];
            i->second.next_path_point =
                metrosystem->lines[i->second.line_index].path[i->second.path_segment_index+1];
            
            auto vec_speed = phys_ptr->getVelocity();
            auto speed = vec_speed.getLength();
            
                // saving speed on turnes
            Vec2 new_vec_speed;
            new_vec_speed.x = i->second.next_path_point.x - i->second.previous_path_point.x;
            new_vec_speed.y = i->second.next_path_point.y - i->second.previous_path_point.y;
            new_vec_speed.normalize();
            
            new_vec_speed.x *= speed;
            new_vec_speed.y *= speed;
            
            phys_ptr->resetForces();
            phys_ptr->setVelocity(new_vec_speed);
            phys_ptr->applyForce(new_vec_speed.getNormalized() * 50.0);
        }
    }
}

bool Gaming_SC::onContactBegins(PhysicsContact& contact)
{
    CCLOG("WASP");
    
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    Vec2 speed1 = nodeA->getPhysicsBody()->getVelocity();
    Vec2 speed2 = nodeB->getPhysicsBody()->getVelocity();
    
    auto train_index1 = nodeA->getTag();
    auto train_index2 = nodeB->getTag();
    
    if(train_index1 + train_index2 > 100)
    {
        MessageBox("P*zdec poezdam", "sledi za dorogoi!");
        
        Director::getInstance()->replaceScene(TransitionSlideInT::create(1.0, MainMenu_SC::createScene()));
    }
    
    CCLOG("%f %f", speed1.length(), speed2.length());
    if(speed1.length() <= 20.0 && speed2.length() <= 20)
    {
        if((nodeA->getTag() == -3000 && nodeB->getTag() != -3000) ||
           (nodeB->getTag() == -3000 && nodeA->getTag() != -3000))
            metrosystem->current_score += 100;
    }
    
    if((nodeA->getTag() == -2000 && nodeB->getTag() != -2000) ||
       (nodeB->getTag() == -2000 && nodeA->getTag() != -2000))
    {
        if(nodeA->getTag() != -2000)
        {
            nodeA->setVisible(false);
            metrosystem->trains.erase(nodeA->getTag());
            nodeA->removeFromParentAndCleanup(true);
        }
        else
        {
            nodeB->setVisible(false);
            metrosystem->trains.erase(nodeB->getTag());
            nodeB->removeFromParentAndCleanup(true);
        }
        
        if(metrosystem->trains_count >= metrosystem->trains_limit
           && metrosystem->trains.size() == 1
           && metrosystem->score_limit > metrosystem->current_score)
        {
            MessageBox("you lost", "kek");
            Director::getInstance()->replaceScene(TransitionSlideInT::create(1.0, MainMenu_SC::createScene()));
        }
        
        else if(metrosystem->trains_count >= metrosystem->trains_limit
                && metrosystem->trains.size() == 0
                && metrosystem->score_limit <= metrosystem->current_score)
        {
            auto center = Director::getInstance()->getVisibleSize();
            center.width /= 2;
            center.height /= 2;
            auto ggwp = Sprite::create("res\\ggwp.jpg");
            ggwp->setPosition(center);
            ggwp->setScale(0.4);
            this->addChild(ggwp);
        }
        
    }

        //bodies can collide
    return true;
}

void Gaming_SC::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
